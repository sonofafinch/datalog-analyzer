/* ---------------------------------------------------------------------------
**
** Rule.h
** Rule class declaration.
** 7-5-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once
#include "Predicate.h"
#include <vector>

class Rule
{
public:
	Rule() {};
	Rule(Predicate nm, vector<Predicate> dt)
	{
		name = nm;
		data = dt;
	}

	string toString()
	{
		unsigned int count = 0;
		stringstream ss;
		ss << name.toString() << " :- ";
		for (vector<Predicate>::iterator it = data.begin(); it != data.end(); ++it)
		{
			ss << it->toString();
			++count;
			if(count < data.size())
			{
				ss << ",";
			}
		}
		return ss.str();
	}

private:
	Predicate name;
	vector<Predicate> data;
};