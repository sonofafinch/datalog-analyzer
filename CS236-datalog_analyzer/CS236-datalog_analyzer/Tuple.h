/* ---------------------------------------------------------------------------
**
** Tuple.h
** Extends the vector class for storing Tuples.
** 7-11-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <string>

template <typename T>
class Tuple : public vector<T>
{
public:
	virtual ~Tuple() {};
};