/* ---------------------------------------------------------------------------
**
** Database.cpp
** Implementation of database and functions to populate it.
** 8-6-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "Database.h"

/*
	Outputs the relations and corresponding facts, sorted by vector < operator as part
	of the set class.
*/
void Database::factOut()
{
	ss << "Fact Evaluation\n\n";
	for (std::map<std::string, Relation>::iterator it = data.begin(); it != data.end(); ++it)
	{
		ss << it->second.getName() << "\n"; //Output relation name
		ss << it->second.toString(); //Output the relation facts
		ss << "\n"; //Blank line after each relation's facts
	}
}

/*
	Takes in a list of schemes created by the Datalog Program, creating an empty relation
	for each.
*/
void Database::evalSchemes(std::vector<Predicate> schm_lst)
{
	for (vector<Predicate>::iterator it = schm_lst.begin(); it != schm_lst.end(); ++it)
	{
		Relation cur_rel;
		cur_rel.setName(it->getName()); //Predicate name becomes the relation name
		cur_rel.setScheme(it->getParams()); //Paremeters are converted to strings and set as scheme
		data.emplace(it->getName(), cur_rel); //The empty relation is added to the database
	}
	ss << "Scheme Evaluation\n\n";
}

/*
	Takes in a vector of predicate facts from the Datalog Program, iterates through
	each and adds them to the relation corresponding to the fact name.
*/
void Database::evalFacts(std::vector<Predicate> fcts_lst)
{
	for (vector<Predicate>::iterator it = fcts_lst.begin(); it != fcts_lst.end(); ++it)
	{
		data.find(it->getName())->second.addFact(it->getParams()); //Find the relation by name and add the fact as a tuple.
	}
	factOut();
}

/*
	Searches a vector of parameters to see if there are multiple entries of one value,
	recording each position in a vector.
*/
vector<int> Database::findTwo(std::vector<Parameter> prms, Parameter cur_param)
{
	std::vector<int> ind;
	int i = 0;
	for (std::vector<Parameter>::iterator it = prms.begin(); it != prms.end(); ++it)
	{
		if (it->toString() == cur_param.toString())
		{
			ind.push_back(i);
		}
		++i;
	}
	return ind;
}

/*
	Check parameters for variable or constant values and select from the current
	relation tuples that match.
*/
void Database::searchSelect(Predicate cur_pred, Relation& cur_rel)
{
	int i = 0; //Count position in list to pass to select function
	vector<Parameter> params = cur_pred.getParam(); //Get the parameters of the current predicate
	for (std::vector<Parameter>::iterator it = params.begin(); it != params.end(); ++it) //Iterate over the parameter list
	{
		if (it->isLiteral()) //Constant?
		{
			cur_rel = cur_rel.select(i, it->toString()); //Add tuples with value in current index
		}
		else //Variable
		{
			vector<int> ind = findTwo(params, *it); //Check for two of the same variable
			if (ind.size() > 1) //If variable reoccurs
			{
				cur_rel = cur_rel.select(ind[0], ind[1]); //Add tuples with matching values at given positions
			}
		}
		++i; //Increment counter
	}
	
}

/*
	Takes a vector of predicates, which are queries generated by the
	Datalog Program, as input and iterates through them to create new relations
	in the database. Each new relation is based on select, project and rename
	operations as required by the query being evaluated.
*/
void Database::evalQueries(std::vector<Predicate> qrs_lst)
{
	ss << "Query Evaluation\n\n";
	for (vector<Predicate>::iterator it = qrs_lst.begin(); it != qrs_lst.end(); ++it) //Iterate through each query
	{
		Relation cur_rel = data.find(it->getName())->second; //Get the relation corresponding to the query
		ss << it->toString() << "? "; //Output the query being examined
		searchSelect(*it, cur_rel); //Select operation on current relation
		if (cur_rel.getList().empty()) //Tuples found?
		{
			ss << "No\n\n";
		}
		else //Yes
		{
			ss << "Yes(" << cur_rel.getList().size() << ")\n"; //Output number of tuples
			ss << "select\n" << cur_rel.toString(); //Output selected facts
			cur_rel = cur_rel.project(it->getParams()); //Project operation on select relation
			ss << "project\n" << cur_rel.toString(); //Output projected facts
			cur_rel = cur_rel.rename(it->getParams()); //Rename based on query variables
			data.emplace(cur_rel.getName(), cur_rel);
			ss << "rename\n" << cur_rel.toString() << "\n"; //Output renamed relation and a blank line
		}
	}
}

/*
	Takes in a predicate and returns a relation based on its variables.
*/
Relation Database::ruleRel(Predicate cur_pred)
{
	std::vector<std::string> cur_params = cur_pred.getParams();
	Relation cur_rel = data.find(cur_pred.getName())->second; //Get the corresponding relation
	searchSelect(cur_pred, cur_rel); //Select based on rule predicate
	if (!(cur_rel.getList().empty())) //Facts found?
	{ //Yes
		cur_rel = cur_rel.project(cur_params); //Project on selected tuples
		cur_rel = cur_rel.rename(cur_pred.getParams()); //Rename based on predicate variables
	}
	return cur_rel;
}

/*
	Keep track of tuples in database to know if rules have altered any relations.
*/
int Database::countTuples()
{
	int cnt = 0;
	for (std::map<std::string, Relation>::iterator it = data.begin(); it != data.end(); ++it)
	{
		std::set<Tuple> cur_fact = it->second.getList();
		for (std::set<Tuple>::iterator jt = cur_fact.begin(); jt != cur_fact.end(); ++jt)
		{
			++cnt;
		}
	}
	return cnt;
}

/*
	Adds the graph to the stringstream
*/
void Database::printGraph(Graph any_graph)
{
	for (int i = 1; i <= any_graph.getSize(); ++i) //Iterate through the graph
	{
		ss << "  R" << i << ": "; //Output the node
		std::set<int> successors = any_graph.getSuccessors(i); //Get node successors
		int j = 0; //Count to track place in set
		for (std::set<int>::iterator it = successors.begin(); it != successors.end(); ++it)
		{
			ss << "R" << *it; //Output successor
			if (j < (successors.size() - 1)) //End of set?
			{
				ss << " "; //No
			}
			++j; //Increment place in set
		}
		ss << "\n";
	}
	ss << "\n";
}

/*
	Prints out the post-order number for each node
*/
void Database::printPO(Graph rev_graph)
{
	ss << "Postorder Numbers\n";
	for (int i = 1; i <= rev_graph.getSize(); ++i) //Iterate through the graph nodes
	{
		ss << "  R" << i << ": " << rev_graph.getPO(i) << "\n"; //Output the node ID and its PO
	}
	ss << "\n";
}

/*
	Make and return the dependency graph and make the reverse dependency graph, passing it out
	by reference, based on the imported rules.
*/
Graph Database::buildGraph(std::vector<Rule> rule_list, Graph& rev_graph)
{
	Graph dep_graph; //New dependency graph
	for (int i = 0; i < rule_list.size; ++i) //Count the rules
	{
		dep_graph.addNode(i + 1); //Add a node for each rule
	}
	rev_graph = dep_graph; //Duplicate the graph for tracking reverse dependencies
	int i, k = 0; //Rule ID's
	for (std::vector<Rule>::iterator it = rule_list.begin(); it != rule_list.end(); ++it) //Iterate through each rule
	{
		++i; //New ID for current rule
		std::vector<Predicate> rule_body = it->getData(); //Get the current rule body predicates
		for (std::vector<Predicate>::iterator jt = rule_body.begin(); jt != rule_body.end(); ++jt) //Iterate through the predicates
		{
			for (std::vector<Rule>::iterator kt = rule_list.begin(); kt != rule_list.end(); ++kt) //Iterate through the rules again
			{
				++k; //Temporary ID for tracking rules
				if ((jt->getName()) == (kt->getHead().getName())) //Does the body predicate match any rule?
				{
					dep_graph.addSuccessor(i, k); //New edge for dependency graph
					rev_graph.addSuccessor(k, i); //New edge for reverse dependency graph
				}
			}
		}
	}
	ss << "Dependency Graph\n";
	printGraph(dep_graph);
	ss << "Reverse Graph\n";
	printGraph(rev_graph);
}

/*
	Performs a depth-first search on a single node.
*/
void Database::DFS(int ID, int& cnt, Graph& rev_graph)
{
	rev_graph.setFlag(ID, true); //Mark the node visited
	std::set<int> sccssrs = rev_graph.getSuccessors(ID); //Get the node's successors
	for (std::set<int>::iterator it = sccssrs.begin(); it != sccssrs.end(); ++it) //Iterate through the neighboring nodes
	{
		if (rev_graph.getFlag(*it) != true) //Node visited yet?
		{
			DFS(*it, cnt, rev_graph); //No, go to the next successor
		}
	}
	++cnt; //Increase the post-order ID
	rev_graph.setPO(ID, cnt); //Set the PO number for the current node
	return;
}

/*
	Performs a depth-first search on a single node without assigning post-order
	numbers, rather assigning connected components to a set.
*/
void Database::DFS(int ID, std::vector<int>& scc, Graph& dep_graph)
{
	scc.push_back(ID); //Add next node visited to the list
	dep_graph.setFlag(ID, true); //Mark the node visited
	std::set<int> sccssrs = dep_graph.getSuccessors(ID); //Get the node's successors
	for (std::set<int>::iterator it = sccssrs.begin(); it != sccssrs.end(); ++it) //Iterate through the neighboring nodes
	{
		if (dep_graph.getFlag(*it) != true) //Node visited yet?
		{
			DFS(*it, scc, dep_graph); //No, go to the next successor
		}
	}
	return;
}

/*
	DFS Forest:  Search through the reverse dependency graph and assign post-order numbers.
*/
void Database::graphDFS(Graph& rev_graph)
{
	int cnt = 0; //Initialize post-order number value
	for (int i = 1; i <= rev_graph.getSize(); ++i) //Iterate through each node
	{
		if (rev_graph.getFlag(i) != true) //Node visited?
		{
			DFS(i, cnt, rev_graph); //No, start a new tree
		}
	}
}

/*
	Finds the strongly connected components in the graph using their
	post-order numbers and returns a map with each set.
*/
std::vector<std::vector<int>> Database::findSCC(Graph& dep_graph, Graph rev_graph)
{
	std::vector<std::vector<int>> scc_list; //List of strongly connected components
	for (int i = rev_graph.getSize(); i > 0; --i) //Look for larger post-order numbers first
	{
		std::vector<int> scc; //Vector to hold path from current node
		DFS(rev_graph.getNode(i), scc, dep_graph); //Search from node w/ highest PO
		if (!scc.empty())
		{
			scc_list.push_back(scc);
		}
	}
	return scc_list; //Return the list of SCC
}

/*
	Run evaluation on specific rules, based on SCC's found
*/
void Database::ruleEval(std::vector<int> scc, std::vector<Rule> rule_list)
{
	ss << "SCC:";
	for (int i = 0; i < scc.size(); ++i) //Iterate through all nodes in the SCC
	{
		ss << " R" << scc[i]; //Output each node
	}
	ss << "\n";
	for (vector<int>::iterator it = scc.begin(); it != scc.end(); ++it) //Iterate through each rule in the SCC
	{
		std::vector<Relation> rule_data; //Temporary vector to save new relations
		std::vector<Predicate> cur_data = rule_list[*it].getData(); //Temporary vector to hold rule data
		for (std::vector<Predicate>::iterator jt = cur_data.begin(); jt != cur_data.end(); ++jt) //Iterate through each predicate
		{
			rule_data.push_back(ruleRel(*jt)); //Save relations created by rule data
		}
		while (rule_data.size() > 1) //More than one new relation?
		{
			rule_data[0] = rule_data[0].join(rule_data[1]); //Join first two relations
			rule_data.erase(rule_data.begin() + 1); //Erase the second
		} //If more than one remains, repeat
		rule_data[0] = rule_data[0].project(rule_data[0].findPos(rule_list[*it].getHead().getParam())); //Project on the remaining relation based on the positions found from rule head
		std::string cur_name = rule_list[*it].getHead().getName();
		data.at(cur_name).compFacts(rule_data[0]); //Remove anything already in the relation prior to printing out
		rule_data[0] = rule_data[0].rename(data.at(cur_name).getScheme()); //Rename based on relation
		ss << rule_data[0].toString(); //Print out prior to union
		data.at(cur_name) = data.at(cur_name).onion(rule_data[0]); //Find the relation with a matching name and union with the one from evaluation
	}
}

/*
	Takes a vector of rules as input and iterates through them to create new relations
	in the database. Each new relation is based on select, project and rename operations
	as required by the rule being evaluated.
*/
void Database::evalRules(std::vector<Rule> rule_list)
{
	ss << "Rule Evaluation\n\n";
	Graph rev_graph; //Initialize the reverse dependency graph
	Graph dep_graph = buildGraph(rule_list, rev_graph); //Build both graphs
	graphDFS(rev_graph); //run DFS Forest on the reverse dependency graph
	printPO(rev_graph); //Print the PO for each node
	std::vector<std::vector<int>> scc_list = findSCC(dep_graph, rev_graph);
	ss << "SCC Search Order\n";
	for (std::vector<std::vector<int>>::iterator it = scc_list.begin(); it != scc_list.end(); ++it)
	{
		std::vector<int> cur_scc = *it;
		ss << "  R" << cur_scc[0] << "\n";
		if ((cur_scc.size() == 1) && (count(cur_scc.begin(), cur_scc.end(), cur_scc[0]) == 1))
		{
			ruleEval(cur_scc, rule_list);
		}
		else
		{
			int cnt = 0;
			do
			{
				cnt = countTuples(); //Set starting tuple count
				ruleEval(cur_scc, rule_list);
			} while (cnt != countTuples()); //Repeat if any relations were altered by rules
		}
	}
	ss << "Rule Evaluation Complete\n\n";
}
