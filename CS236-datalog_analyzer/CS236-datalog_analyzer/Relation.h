/* ---------------------------------------------------------------------------
**
** Relation.h
** Pulls relationships from parameters.
** 7-22-16
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

class Relation
{
public:
	Relation() {}

	Relation select(int index, std::string value);
	Relation select(int ind1, int ind2);
	Relation project(int ind1, int ind2);
	Relation rename();
	void addFact(std::vector<std::string> fcts);
	void factSort();
	void setName(std::string nm)
	{
		name = nm;
	}
	void setScheme(std::vector<std::string> prms)
	{
		for (std::vector<std::string>::iterator it = prms.begin(); it != prms.end(); ++it)
		{
			rel_scheme.push_back(*it);
		}
	}
	string toString()
	{
		stringstream ss;
		ss << name << "\n\n";
		for (std::set<Tuple>::iterator it = fact_list.begin(); it != fact_list.end(); ++it)
		{
			Tuple cur_tuple = *it;
			for (int i = 0; i < it->size(); ++i)
			{
				ss << "  " << rel_scheme[i] << "=" << cur_tuple[i];
				if (i < it->size() - 1)
				{
					ss << " ";
				}
			}
			ss << "\n";
		}
		ss << "\n";
		return ss.str();
	}

private:
	Scheme rel_scheme;
	std::string name;
	std::set<Tuple> fact_list;
	std::vector<Predicate> query_list;
};