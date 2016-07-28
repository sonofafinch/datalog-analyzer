/* ---------------------------------------------------------------------------
**
** Parameter.h
** Parameter class declaration.
** 7-28-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once
#include <string>

using namespace std;

class Parameter
{
public:
	Parameter()
	{
		literal = false;
	};
	Parameter(bool ltrl, string nm, string vl)
	{
		literal = ltrl;
		name = nm;
		value = vl;
	}

	bool isLiteral()
	{
		return literal;
	}

	string toString()
	{
		return value;
	}

private:
	bool literal;
	string name;
	string value;
};
