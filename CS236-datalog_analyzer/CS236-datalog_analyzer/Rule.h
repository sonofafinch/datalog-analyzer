/* ---------------------------------------------------------------------------
**
** Rule.h
** Rule class declaration.
** 7-30-16
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
		head = nm;
		data = dt;
	}

	Predicate getHead()
	{
		return head;
	}
	vector<Predicate> getData()
	{
		return data;
	}
	string toString()
	{
		unsigned int count = 0;
		stringstream ss;
		ss << head.toString() << " :- ";
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
	Predicate head;
	vector<Predicate> data;
};