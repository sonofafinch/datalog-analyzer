/* ---------------------------------------------------------------------------
**
** Relation.cpp
** Pulls relationships from parameters.
** 7-22-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "Relation.h"

Relation Relation::select(int index, std::string value)
{

}

Relation Relation::select(int ind1, int ind2)
{

}

Relation Relation::project(int ind1, int ind2)
{

}

Relation Relation::rename()
{

}

void Relation::addFact(std::vector<std::string> fcts)
{
	fact_list.emplace(fcts);
}

void Relation::factSort()
{
	for (std::set<Tuple>::iterator it = fact_list.begin(); it != fact_list.end(); ++it)
	{
		sort(it->begin(), it->end());
	}
}