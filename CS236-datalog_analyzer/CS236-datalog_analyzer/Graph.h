/* ---------------------------------------------------------------------------
**
** Graph.h
** Graph class declaration. Used for storing rules for evaluation.
** 8-6-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once

#include <map>
#include <set>

class Graph
{
public:
	Graph(){}

	void addNode(int cur_ID);
	void addSuccessor(int ID1, int ID2);
	void setPO(int ID, int po_num);
	void setFlag(int ID, bool status);

	/*
		Reports on whether the node has been seen during
		search.
	*/
	bool getFlag(int ID)
	{
		return graph.at(ID).visit_flag;
	}
	/*
		Returns the list of nodes that are next to the current one.
	*/
	std::set<int> getSuccessors(int ID)
	{
		return graph.at(ID).successors;
	}
	/*
		Returns the map size for keeping track of nodes in the graph.
	*/
	int getSize()
	{
		return graph.size();
	}
	/*
		Find a specific node based on its post-order number.
	*/
	int getNode(int po_num)
	{
		for (std::map<int, Node>::iterator it = graph.begin(); it != graph.end(); ++it)
		{
			if (it->second.postorder_num == po_num)
			{
				return it->first;
			}
		}
		return 0;
	}
	/*
		Returns the post-order number for a specific node
	*/
	int getPO(int ID)
	{
		return graph.at(ID).postorder_num;
	}

private:
	class Node
	{
	public:
		Node()
		{
			postorder_num, visit_flag = 0;
		}

		std::set<int> successors;
		int postorder_num;
		bool visit_flag;
	};
	std::map<int, Node> graph;
};