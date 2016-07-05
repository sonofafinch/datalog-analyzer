/* ---------------------------------------------------------------------------
**
** Predicate.h
** Predicate class declaration.
** 7-4-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once
#include "Parameter.h"
#include <sstream>
#include <vector>

using namespace std;

class Predicate
{
public:
	Predicate() {};
	Predicate(string nm, vector<Parameter> prmtrs)
	{
		name = nm;
		parameters = prmtrs;
	}

	string toString()
	{
		int count = 0;
		stringstream ss;
		ss << name << "(";
		for (vector<Parameter>::iterator it = parameters.begin(); it != parameters.end(); ++it)
		{
			ss << it->toString();
			++count;
			if (count < parameters.size())
			{
				ss << ",";
			}
		}
		ss << ")";
	}

private:
	string name;
	vector<Parameter> parameters;
};