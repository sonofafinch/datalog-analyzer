/* ---------------------------------------------------------------------------
**
** Database.cpp
** Implementation of database and functions to populate it.
** 7-22-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "Database.h"

void Database::schemeOut()
{
	ss << "Scheme Evaluation\n\n";
}

void Database::factOut()
{
	ss << "Fact Evaluation\n\n";
	for (std::map<std::string, Relation>::iterator it = data.begin(); it != data.end(); ++it)
	{
		it->second.factSort();
		ss << it->second.toString();
	}
}

void Database::evalSchemes(std::vector<Predicate> schm_lst)
{
	for (vector<Predicate>::iterator it = schm_lst.begin(); it != schm_lst.end(); ++it)
	{
		Relation cur_rel;
		cur_rel.setName(it->getName());
		cur_rel.setScheme(it->getParams());
		data.emplace(it->getName(), cur_rel);
	}
	schemeOut();
}

void Database::evalFacts(std::vector<Predicate> fcts_lst)
{
	for (vector<Predicate>::iterator it = fcts_lst.begin(); it != fcts_lst.end(); ++it)
	{
		data.find(it->getName())->second.addFact(it->getParams());
	}
	factOut();
}