/* ---------------------------------------------------------------------------
**
** Tuple.h
** Extends the vector class for storing Tuples.
** 7-22-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <string>

class Tuple : public std::vector<std::string>
{
public:
	virtual ~Tuple() {};
};