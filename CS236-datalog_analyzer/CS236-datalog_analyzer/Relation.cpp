/* ---------------------------------------------------------------------------
**
** Relation.cpp
** Pulls relationships from parameters.
** 8-3-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "Relation.h"

/*
	Takes an index as a position in a vector and a string literal as inputs. The
	current list of facts is searched for facts with literals in the index/position.
*/
Relation Relation::select(int index, std::string value)
{
	Relation next_rel;
	next_rel.setName("select");
	next_rel.setScheme(rel_scheme);
	for (std::set<Tuple>::iterator it = fact_list.begin(); it != fact_list.end(); ++it)
	{
		Tuple cur_tuple = *it;
		if (value == cur_tuple[index])
		{
			next_rel.addFact(cur_tuple);
		}
	}
	return next_rel;
}

/*
	Takes two indices as inputs. The current list of facts is searched for
	facts with matching elements at the two positions.
*/
Relation Relation::select(int ind1, int ind2)
{
	Relation next_rel;
	next_rel.setName("select");
	next_rel.setScheme(rel_scheme);
	for (std::set<Tuple>::iterator it = fact_list.begin(); it != fact_list.end(); ++it)
	{
		Tuple cur_tuple = *it;
		if (cur_tuple[ind1] == cur_tuple[ind2])
		{
			next_rel.addFact(cur_tuple);
		}
	}
	return next_rel;
}

/*
	Returns a relation with tuples that have only those variables indicated in the
	query which created them. Takes in a vector of positions for variables within
	facts.
*/
Relation Relation::project(std::vector<std::string> vals)
{
	Relation next_rel; //Temporary Relation
	next_rel.setName("project"); //Named after operation
	std::vector<int> pos = findPos(vals); //Variables positions
	std::vector<string> schm; //Temporary vector to set scheme with
	for (unsigned int i = 0; i < pos.size(); ++i) //Iterate through scheme to get variables in query
	{
		schm.push_back(rel_scheme[pos[i]]); //Push each variable in order to the new vector
	}
	next_rel.setScheme(schm); //Set scheme with columns being kept
	for (std::set<Tuple>::iterator it = fact_list.begin(); it != fact_list.end(); ++it) //Iterate through selected facts
	{
		Tuple cur_tuple; //New fact based on query
		Tuple fct_tuple = *it; //Selected fact
		for (unsigned int i = 0; i < pos.size(); ++i) //Iterate through positions
		{
			cur_tuple.push_back(fct_tuple[pos[i]]); //Put the variable in position in the new fact
		}
		next_rel.addFact(cur_tuple); //Add the new tuple to the new relation
	}
	return next_rel; 
}

Relation Relation::project(std::vector<int> pos)
{
	Relation next_rel; //Temporary Relation
	next_rel.setName("project"); //Named after operation
	std::vector<string> schm; //Temporary vector to set scheme with
	for (unsigned int i = 0; i < pos.size(); ++i) //Iterate through scheme to get variables in query
	{
		schm.push_back(rel_scheme[pos[i]]); //Push each variable in order to the new vector
	}
	next_rel.setScheme(schm); //Set scheme with columns being kept
	for (std::set<Tuple>::iterator it = fact_list.begin(); it != fact_list.end(); ++it) //Iterate through selected facts
	{
		Tuple cur_tuple; //New fact based on query
		Tuple fct_tuple = *it; //Selected fact
		for (unsigned int i = 0; i < pos.size(); ++i) //Iterate through positions
		{
			cur_tuple.push_back(fct_tuple[pos[i]]); //Put the variable in position in the new fact
		}
		next_rel.addFact(cur_tuple); //Add the new tuple to the new relation
	}
	return next_rel;
}

/*
	Returns a new relation with an updated name based on the query being evaluated.
*/
Relation Relation::rename(std::vector<std::string> vars)
{
	Relation fnl_rltn; //New relation to be returned
	stringstream ss;
	std::vector<std::string> new_var;
	for (std::vector<std::string>::iterator it = vars.begin(); it != vars.end(); ++it) //Iterate through variables
	{
		string cur_var = *it; //Temporary string for comparison
		if (cur_var[0] != '\'') //Ignore literals
		{
			new_var.push_back(*it);
			ss << *it; //Add each variable to the stringstream
		}
	}
	fnl_rltn.name = ss.str(); //Name based on variables in query
	fnl_rltn.setScheme(new_var); //Confirm scheme based on query
	fnl_rltn.fact_list = fact_list; //Copy facts that remain after project
	return fnl_rltn; 
}

/*
	Combines two relations, regardless of their similarities.
*/
Relation Relation::join(Relation r2)
{
	Relation nw_rltn; //Temporary relation for combining inputs
	std::vector<std::pair<int, int>> matches; //Vector to hold matching pair values
	Scheme new_scheme = combScheme(getScheme(), r2.getScheme(), matches); //New scheme based on two relations
	nw_rltn.setScheme(new_scheme); //Set new relation scheme
	std::set<Tuple> cur_tuples = getList(); // Temporary set to iterate through
	std::set<Tuple> comp_tuples = r2.getList(); //Set to compare with
	for (std::set<Tuple>::iterator it = cur_tuples.begin(); it != cur_tuples.end(); ++it) //Iterate through current relation facts
	{
		for (std::set<Tuple>::iterator jt = comp_tuples.begin(); jt != comp_tuples.end(); ++jt) //Compare with passed in relation facts
		{
			if (testTuples(*it, *jt, matches)) //Compatible?
			{
				nw_rltn.addFact(joinTuple(*it, *jt, matches)); //Join current tuples if possible
			}
		}
	}
	return nw_rltn;
}

/*
	Union operation to combine relations with matching name/scheme.
*/
Relation Relation::onion(Relation r3)
{
	Relation fnl_rltn; //New relation based of union
	if ((rel_scheme.size() == r3.getScheme().size()))
	{
		fnl_rltn.name = name; //Same name
		fnl_rltn.rel_scheme = rel_scheme; //Same scheme
		std::set<Tuple> cur_list = r3.getList(); //Temporary set to iterate through
		for (std::set<Tuple>::iterator it = cur_list.begin(); it != cur_list.end(); ++it) //Iterate through new facts
		{
			fact_list.emplace(*it); //Add to current relation list
		}
		fnl_rltn.fact_list = fact_list; //Copy facts to new relation
	}
	return fnl_rltn; //Return new relation
}

/*
	Discovers the positions in the scheme that match the query variables.
*/
std::vector<int> Relation::findPos(std::vector<std::string> vals)
{
	std::vector<int> pos; //List of positions to use for new relation facts
	std::vector<std::string> seen; //List of parameters seen
	for (unsigned int i = 0; i < vals.size(); ++i) //Iterate through the query
	{
		if((vals[i][0] != '\'') && (find(seen.begin(), seen.end(), vals[i]) == seen.end())) //Variable?
		{
			pos.push_back(i); //Keep current position
		}
		seen.push_back(vals[i]); //Mark current parameter as seen
	}
	return pos;
}

/*
	Second option for finding positions is for rearranging columns of relations to
	fit rule evaluation scheme.
*/
std::vector<int> Relation::findPos(std::vector<Parameter> vars)
{
	std::vector<int> pos; //List of positions to use for new relation facts
	for (unsigned int i = 0; i < vars.size(); ++i) //Iterate through the query
	{
		for (unsigned int j = 0; j < rel_scheme.size(); ++j) //Iterate through scheme
		{
			if (rel_scheme[j] == vars[i].toString()) //Match?
			{
				pos.push_back(j); //Keep specific column
			}
		}
	}
	return pos;
}

/*
	Combines two schemes and returns a new one.
*/
Scheme Relation::combScheme(Scheme s1, Scheme s2, std::vector<std::pair<int, int>>& matches)
{
	Scheme nw_schm;
	for (Scheme::iterator it = s1.begin(); it != s1.end(); ++it)
	{
		nw_schm.push_back(*it);
	}
	for (unsigned int i = 0; i < s2.size(); ++i)
	{
		bool found = false;
		for (unsigned int j = 0; j < nw_schm.size(); ++j)
		{
			if (s2[i] == nw_schm[j])
			{
				matches.push_back(make_pair(j, i));
				found = true;
			}
		}
		if (found == false)
		{
			nw_schm.push_back(s2[i]);
		}
	}
	return nw_schm;
}

/*
	Test to see if tuples are join compatible, that their shared variables match.
*/
bool Relation::testTuples(Tuple t1, Tuple t2, std::vector<std::pair<int, int>> matches)
{
	for (std::vector<std::pair<int, int>>::iterator it = matches.begin(); it != matches.end(); ++it)
	{
		if ((it->first < t1.size()) && (it->second < t2.size()))
		{
			std::string one_spot = t1[it->first]; //First relation fact variable
			std::string two_spot = t2[it->second]; //Second relation fact variable
			if (one_spot.compare(two_spot) != 0) //Strings match?
			{
				return false; //No
			}
		}
		else
		{
			return false;
		}
	}
	return true; //All Match
}

/*
	Places all of the first tuple's values in a new tuple and adds unique values from second
	tuple afterward. Returns new tuple with combined values.
*/
Tuple Relation::joinTuple(Tuple t1, Tuple t2, std::vector<std::pair<int, int>> matches)
{
	Tuple t; //Temporary tuple to hold combination of two inputs
	for (Tuple::iterator it = t1.begin(); it != t1.end(); ++it) //Iterate through the first tuple
	{
		t.push_back(*it); //Place all values in new tuple
	}
	for (unsigned int i = 0; i < t2.size(); ++i) //Iterate through second tuple
	{
		int cur_pair = i; //Temporary value for comparing match second position
		bool found = false; //Bell for found positions
		for (std::vector<std::pair<int, int>>::iterator it = matches.begin(); it != matches.end(); ++it) //Search matches
		{
			if (it->second == cur_pair) //Found?
			{
				found = true; //Yes
			}
		}
		if (found == false) //Is the value already in the tuple?
		{
			t.push_back(t2[i]); //No
		}
	}
	return t;
}

/*
	Checks to see if the fact already exists and removes it from the temporary relation if so.
 */
void Relation::compFacts(Relation& r4)
{
	std::set<Tuple> cur_facts = r4.getList();
	for(std::set<Tuple>::iterator it = cur_facts.begin(); it != cur_facts.end(); ++it)
	{
		if(fact_list.find(*it) != fact_list.end())
		{
			r4.fact_list.erase(*it);
		}
	}
}

/*
	Takes in a vector of strings to place into a set of tuples, the latter being
	equivalent to a tuple. The set can't contain duplicate entries.
*/
void Relation::addFact(std::vector<std::string> fcts)
{
	Tuple cur_tuple; //Temporary tuple for converting from vector
	for (std::vector<std::string>::iterator it = fcts.begin(); it != fcts.end(); ++it) //Iterate through vector
	{
		cur_tuple.push_back(*it); //Add each vector element to temp tuple
	}
	fact_list.emplace(cur_tuple); //Add temp tuple as fact in current relation
}

/*
	Takes in a string to be used for the relation's new name.
*/
void Relation::setName(std::string nm)
{
	name = nm;
}

/*
	Takes in a vector of parameters, having been converted to strings, to be copied
	over as the scheme for the relation.
*/
void Relation::setScheme(std::vector<std::string> prms)
{
	rel_scheme.clear();
	for (std::vector<std::string>::iterator it = prms.begin(); it != prms.end(); ++it)
	{
		rel_scheme.push_back(*it);
	}
}
