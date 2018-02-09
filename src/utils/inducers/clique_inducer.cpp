/*
 * clique_inducer.cpp
 *
 *  Created on: Jan 28, 2018
 *      Author: liz
 */


#include "clique_inducer.h"
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;
using namespace std;

template <typename Graph>
CliqueInducer<Graph>::CliqueInducer(int n) {
	clique_size = n;

}

template <typename Graph>
bool CliqueInducer<Graph>::IsInduced(const Graph& g, const Graph& subgraph){
	// todo Improve logic: find the largest clique (instead of testing clique candidates)

	typedef typename graph_traits<Graph>::vertex_iterator vertex_iter;

	vector<int> clique_vertex_candidates;
	int subgraph_v = boost::num_vertices(subgraph);
	bool induced = false;

	std::pair<vertex_iter, vertex_iter> vp;
	for (vp = vertices(g); vp.first != vp.second; ++vp.first)
	{
		if (degree(*vp.first, g) >= clique_size - 1)
			clique_vertex_candidates.push_back(*vp.first);
	}

	vector< vector<int> > subsets = this->SubsetsOfSize(clique_vertex_candidates,subgraph_v);

	for (int i = 0; i < subsets.size() && !induced; i++)
	{
		vector<int> subset = subsets[i];
		induced  = this->IsSubsetInduced(g, subgraph, subset);
	}

	return induced;
}


