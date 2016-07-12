/* ---------------------------------------------------------------------------
**
** Scheme.h
** Extends the vector class for storing Schemes.
** 7-11-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <string>

template <typename T>
class Scheme : public vector<T>
{
public:
	virtual ~Scheme() {};
};