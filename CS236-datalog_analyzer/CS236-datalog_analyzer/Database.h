/* ---------------------------------------------------------------------------
**
** Database.h
** Stores relationships for use by the interpreter.
** 7-22-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once

#include "Relation.h"
#include <sstream>
#include <map>
#include <string>

class Database
{
public:
	Database() {}

	void schemeOut();
	void factOut();
	void evalSchemes(std::vector<Predicate> schm_lst);
	void evalFacts(std::vector<Predicate> fcts_lst);

private:
	stringstream ss;
	std::map<std::string, Relation> data;
};