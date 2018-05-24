/*
 * graph_examiner.h
 *
 *  Created on: 2018 M02 15
 *      Author: Liz
 */

#ifndef GRAPH_EXAMINER_H_
#define GRAPH_EXAMINER_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <iostream>
#include "graph/subgraph_factory.cpp"
#include "graph_generator.cpp"
#include "induced_check.cpp"
#include "utils/colorer.cpp"
#include "utils/hole_detector.cpp"

using namespace std;
using namespace boost;

class GraphExaminer{
	typedef  boost::adjacency_list<vecS, vecS, undirectedS> Graph;
	typedef  boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typedef  boost::graph_traits<Graph>::vertices_size_type vertices_size_type;
	typedef  boost::property_map<Graph, vertex_index_t>::const_type vertex_index_map;
	typedef std::pair<int, int> E;

public:
	GraphExaminer(){graph_count_ = 0; };
	void Examine(int max_v, int color_limit, vector<string> subgraphs);
private:
	int graph_count_;
	queue<Graph> graphs_queue_;
	GraphGenerator<Graph> gen;
	InducedCheck<Graph> checker;
	HoleDetector<Graph> hd;
	Colorer<Graph> colorer;

	//vector<Gragh> vg_;

	void PrintGraph(const Graph& g);
	void PrintDegrees(const Graph& g);
	void CheckGraphs(vector<Graph> graphs, int color_limit);
	bool CheckGraph(const Graph& g, int color_limit);
	void PrintColoring(map<int, int> color_map);
};


#endif /* GRAPH_EXAMINER_H_ */
