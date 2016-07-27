/* ---------------------------------------------------------------------------
**
** Relation.cpp
** Pulls relationships from parameters.
** 7-26-16
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
	std::vector<string> fcts; //Temporary vector to set scheme with
	for (int i = 0; i < pos.size(); ++i) //Iterate through scheme to get variables in query
	{
		fcts.push_back(rel_scheme[pos[i]]); //Push each variable in order to the new vector
	}
	next_rel.setScheme(fcts); //Set scheme with columns being kept
	for (std::set<Tuple>::iterator it = fact_list.begin(); it != fact_list.end(); ++it) //Iterate through selected facts
	{
		Tuple cur_tuple; //New fact based on query
		Tuple fct_tuple = *it; //Selected fact
		for (int i = 0; i < pos.size(); ++i) //Iterate through positions
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
Relation Relation::rename(std::vector<std::string> vars, Relation cur_rel)
{
	Relation fnl_rltn; //New relation to be returned
	stringstream ss;
	for (std::vector<std::string>::iterator it = vars.begin(); it != vars.end(); ++it) //Iterate through variables
	{
		ss << *it; //Add each variable to the stringstream
	}
	fnl_rltn.name = ss.str(); //Name based on variables in query
	fnl_rltn.setScheme(vars); //Confirm scheme based on query
	fnl_rltn.fact_list = cur_rel.fact_list; //Copy facts that remain after project
	return fnl_rltn; 
}

/*
	Discovers the positions in the scheme that match the query variables.
*/
std::vector<int> Relation::findPos(std::vector<std::string> vals)
{
	std::vector<int> pos;
	for (int i = 0; i < vals.size(); ++i) //Columns to keep
	{
		for (int j = 0; j < rel_scheme.size(); ++j)
		{
			if (rel_scheme[j] == vals[i])
			{
				pos.push_back(j);
			}
		}
	}
	return pos;
}

/*
	Takes in a vector of strings to place into a set of tuples, the latter being
	equivalent to a tuple. The set can't contain duplcate entries.
*/
void Relation::addFact(std::vector<std::string> fcts)
{
	fact_list.emplace(fcts);
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