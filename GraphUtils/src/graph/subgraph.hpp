/*
 * SubGraph.h
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#ifndef GRAPH_SUBGRAPH_HPP_
#define GRAPH_SUBGRAPH_HPP_

#include <string>
#include "../induced/basic_inducer.hpp"

template <typename Graph>
class Subgraph {
public:
	Graph graph;
	BasicInducer<Graph>* inducer;
	string name;

	Subgraph(Graph g, BasicInducer<Graph>* ind, const string& n)
	{
		graph = g;
		inducer = ind;
		name = n;
	}

	bool IsInduced(const Graph& g){
		return inducer->IsInduced(g, graph);

	}

private:

};
#endif /* GRAPH_SUBGRAPH_HPP_ */
