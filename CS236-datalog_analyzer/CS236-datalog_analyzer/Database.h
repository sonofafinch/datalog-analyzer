/* ---------------------------------------------------------------------------
**
** Database.h
** Stores relationships for use by the interpreter.
** 7-30-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once

#include "Relation.h"
#include "Parameter.h"
#include "Rule.h"
#include <sstream>
#include <map>
#include <string>

class Database
{
public:
	Database() {}

	void factOut();
	void evalSchemes(std::vector<Predicate> schm_lst);
	void evalFacts(std::vector<Predicate> fcts_lst);
	std::vector<int> findTwo(std::vector<Parameter> prms, Parameter cur_param);
	void searchSelect(Predicate cur_pred, Relation& cur_rel);
	void evalQueries(std::vector<Predicate> qrs_lst);
	Relation ruleRel(Predicate cur_pred);
	int countTuples();
	void evalRules(std::vector<Rule> rule_list);
	string toString()
	{
		return ss.str();
	}

private:
	stringstream ss;
	std::map<std::string, Relation> data;
};