/*
 * max_vertex_inducer.h
 *
 *  Created on: Jan 28, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_DOMINATING_VERTEX_INDUCER_H_
#define UTILS_INDUCERS_DOMINATING_VERTEX_INDUCER_H_

#include "../inducer/basic_inducer.hpp"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

//todo: Extend to dominating clique

template <typename Graph>
class DominatingVertexInducer: public BasicInducer<Graph> {
public:
	DominatingVertexInducer(int n);
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	int max_degree;
};

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
	int g_v = boost::num_vertices(g);
	bool induced = false;

	if(g_v == subgraph_v && boost::num_edges(g) == boost::num_edges(subgraph)){
			return this->IsIsomorphic(g, subgraph);
		}

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

		// todo: replace with basic inducer method
		large_vertex_neighbors.push_back(large_vertices[i]);
		n = adjacent_vertices(large_vertices[i], g);
		for(; n.first != n.second; ++n.first){
			large_vertex_neighbors.push_back(*n.first);
		}

		int last_id = g_v - 1;

		// make sure the last vertex is either the largest or in the neighborhood
		if (large_vertices[i] == last_id || find (large_vertex_neighbors.begin(), large_vertex_neighbors.end(), last_id) != large_vertex_neighbors.end())
		{
			vector< vector<int> > subsets = this->SubsetsOfSize(large_vertex_neighbors,subgraph_v);

			for (int i = 0; i < subsets.size() && !induced; i++)
			{
				vector<int> subset = subsets[i];
				induced  = this->IsSubsetIsomorpic(g, subgraph, subset);
			}
		}
	}


	// find all vertices with degrees > max_degree
	// get all their neighbours and create all subsets with correct size
	// test if they are isomorphic

	return induced;
}

#endif /* UTILS_INDUCERS_DOMINATING_VERTEX_INDUCER_H_ */
