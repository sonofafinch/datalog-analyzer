/* ---------------------------------------------------------------------------
**
** Relation.h
** Pulls relationships from parameters.
** 8-4-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once

#include "Tuple.h"
#include "Scheme.h"
#include "Predicate.h"
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <utility>

class Relation
{
public:
	Relation() {}

	Relation select(int index, std::string value);
	Relation select(int ind1, int ind2);
	Relation project(std::vector<std::string> vals);
	Relation project(std::vector<int> pos);
	Relation rename(std::vector<std::string> vars);
	Relation join(Relation r2);
	Relation onion(Relation r3);
	std::vector<int> findPos(std::vector<std::string> vals);
	std::vector<int> findPos(std::vector<Parameter> vars);
	Scheme combScheme(Scheme schm1, Scheme schm2, std::vector<std::pair<int, int>>& matches);
	bool testTuples(Tuple t1, Tuple t2, std::vector<std::pair<int, int>> matches);
	Tuple joinTuple(Tuple t1, Tuple t2, std::vector<std::pair<int, int>> matches);
	void compFacts(Relation& r4);
	void addFact(std::vector<std::string> fcts);
	void setName(std::string nm);
	void setScheme(std::vector<std::string> prms);
	set<Tuple> getList()
	{
		return fact_list;
	}
	string getName()
	{
		return name;
	}
	Scheme getScheme()
	{
		return rel_scheme;
	}
	/*
		Outputs the relation facts only, without name, with scheme association
	*/
	std::string toString()
	{
		stringstream ss;
		for (std::set<Tuple>::iterator it = fact_list.begin(); it != fact_list.end(); ++it)
		{
			Tuple cur_tuple = *it; //Temp tuple equal to tuple iterator points to
			if (cur_tuple.size() > 0)
			{
				ss << "  "; //Indent two spaces for each line
				for (unsigned int i = 0; i < cur_tuple.size(); ++i) //Iterate through the vector of strings
				{
					ss << rel_scheme[i] << "=" << cur_tuple[i]; //Column = row
					if (i < it->size() - 1) //If there are other vector elements remaining, add space between them
					{
						ss << " ";
					}
				}
				ss << "\n"; //Each tuple on its own line
			}
		}
		return ss.str();
	}

private:
	Scheme rel_scheme;
	std::string name;
	std::set<Tuple> fact_list;
};
