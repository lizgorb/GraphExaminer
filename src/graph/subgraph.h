/*
 * SubGraph.h
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#ifndef GRAPH_SUBGRAPH_H_
#define GRAPH_SUBGRAPH_H_

#include "../utils/inducers/basic_inducer.cpp"
#include <string>

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

	bool IsInduced(const Graph& g);
private:

};
#endif /* GRAPH_SUBGRAPH_H_ */
