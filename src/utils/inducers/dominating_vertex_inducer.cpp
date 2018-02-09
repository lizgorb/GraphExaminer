/*
 * max_vertex_inducer.cpp
 *
 *  Created on: Jan 28, 2018
 *      Author: liz
 */

#include "dominating_vertex_inducer.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

template <typename Graph>
DominatingVertexInducer<Graph>::DominatingVertexInducer(int n) {
	max_degree = n;
}

template <typename Graph>
bool DominatingVertexInducer<Graph>::IsInduced(const Graph& g, const Graph& subgraph)
{
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iter;
	typedef typename boost::graph_traits<Graph>::adjacency_iterator adjacency_iterator;

	vector<int> large_vertices;
	int subgraph_v = boost::num_vertices(subgraph);
	bool induced = false;

	std::pair<vertex_iter, vertex_iter> vp;
	for (vp = vertices(g); vp.first != vp.second; ++vp.first)
	{
		if (degree(*vp.first, g) >= max_degree)
			large_vertices.push_back(*vp.first);
	}

	// for every large vertex
	vector<int> large_vertex_neighbors;
	std::pair<adjacency_iterator,adjacency_iterator> n;

	for(int i=0; i < large_vertices.size() && !induced; i++){
		large_vertex_neighbors.clear();

		large_vertex_neighbors.push_back(large_vertices[i]);
		n = adjacent_vertices(large_vertices[i], g);
		for(; n.first != n.second; ++n.first){
			large_vertex_neighbors.push_back(*n.first);
		}

		vector< vector<int> > subsets = this->SubsetsOfSize(large_vertex_neighbors,subgraph_v);

		for (int i = 0; i < subsets.size() && !induced; i++)
		{
			vector<int> subset = subsets[i];
			induced  = this->IsSubsetInduced(g, subgraph, subset);
		}
	}


	// find all vertices with degrees > max_degree
	// get all their neighbours and create all subsets with correct size
	// test if they are isomorphic

	return induced;
}



