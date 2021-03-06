/* ---------------------------------------------------------------------------
**
** Predicate.h
** Predicate class declaration.
** 7-26-16
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
	Predicate() {}
	Predicate(string nm, vector<Parameter> prmtrs)
	{
		name = nm;
		parameters = prmtrs;
	}

	string toString()
	{
		unsigned int count = 0;
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
		return ss.str();
	}
	string getName()
	{
		return name;
	}

	/*
		Converts the parameters to strings, as part of a vector, which gets returned.
	*/
	vector<string> getParams()
	{
		vector<string> params;
		for (vector<Parameter>::iterator it = parameters.begin(); it != parameters.end(); ++it)
		{
			params.push_back(it->toString());
		}
		return params;
	}

	vector<Parameter> getParam()
	{
		return parameters;
	}

private:
	string name;
	vector<Parameter> parameters;
};