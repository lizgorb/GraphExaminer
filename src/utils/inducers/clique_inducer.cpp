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
	int subgraph_v = boost::num_vertices(subgraph);
	bool induced = false;

	if (subgraph_v == 4){
		return IsInducedK4(g);
	}

	vector<int> clique_sizes;
	// Instantiate the visitor for printing cliques
	InducedCliqueVisitor vis(clique_sizes);

	// Use the Bron-Kerbosch algorithm to find all cliques, printing them
	// as they are found.
	bron_kerbosch_all_cliques(g, vis);

	int max_clique = vis.max_clique_size();
	if (max_clique >= subgraph_v){
		induced = true;
	}

/*
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iter;
	vector<int> clique_vertex_candidates;
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
*/
	return induced;
}


template <typename Graph>
bool CliqueInducer<Graph>::IsInducedK4(const Graph& g){
	bool induced  = false;
	int i = num_vertices(g) - 1;

	// todo: this is similar to diamond inducer - combine the two somehow

	// induce the neighborhood of the last vertex
	vector<int> neighbors = this->GetNeighborhood(g, i);

	// is there is a triangle in the neighborhood we have a k4
	if (neighbors.size() >= 3){
		Graph ngbGraph = this->InduceSubset(g, neighbors);

		vector<vector<int>> triangles;
		vector<int> parents(neighbors.size(), -1);

		TriangleVisitor vis(triangles, parents);
		depth_first_search(ngbGraph, visitor(vis));

		induced = vis.HasTriangles();
	}

	return induced;
}
