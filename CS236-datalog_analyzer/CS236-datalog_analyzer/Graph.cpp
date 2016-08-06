/* ---------------------------------------------------------------------------
**
** Graph.cpp
** Graph class definition.
** 8-6-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "Graph.h"

/*
	Adds an empty node with to the map with a unique identifier.
*/
void Graph::addNode(int cur_ID)
{
	Node new_node;
	graph.emplace(cur_ID, new_node);
}

/*
	Adds edges from the current node to sucessor nodes
*/
void Graph::addSuccessor(int ID1, int ID2)
{
	graph.at(ID1).successors.emplace(ID2);
}

/*
	Sets the post-order number from one passed in as part of the
	depth-first search of the graph
*/
void Graph::setPO(int ID, int po_num)
{
	graph.at(ID).postorder_num = po_num;
}

/*
	Sets the visit flag based on DFS function in Database.
*/
void Graph::setFlag(int ID, bool status)
{
	graph.at(ID).visit_flag = status;
}