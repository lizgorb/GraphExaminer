/*
 * clique_inducer.h
 *
 *  Created on: Jan 28, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_CLIQUE_INDUCER_H_
#define UTILS_INDUCERS_CLIQUE_INDUCER_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../inducer/basic_inducer.hpp"

using namespace boost;
using namespace std;

class TriangleVisitor : public boost::default_dfs_visitor {
public:
	TriangleVisitor(vector<vector<int>>& triangles, vector<int>& parents) :
		triangles_(triangles), parents_(parents)  { }

	template < typename Edge, typename Graph >
	void tree_edge(Edge e, const Graph &g)
	{
		int s = source(e, g);
		int t = target(e, g);
		if(parents_[t] == -1){
			parents_[t] = s;
		}
	}

    template < typename Edge, typename Graph >
    void back_edge(Edge e, const Graph &g)
    {
    	int s = source(e, g);
    	int t = target(e, g);
    	if(parents_[t] != s && parents_[parents_[s]] == t){
    		vector<int> triangle = {parents_[s], s, t};
    		triangles_.push_back(triangle);
    	}
    }

    bool HasTriangles()  {
    	return triangles_.size() > 0;
    }
private:
    vector<int>& parents_;
    vector<vector<int>>& triangles_;
};

class InducedCliqueVisitor
{
private:
	 vector<int>& clique_sizes_;

public:
	 InducedCliqueVisitor(vector<int>& clique_sizes): clique_sizes_(clique_sizes) {}

    int max_clique_size(){return *max_element(std::begin(clique_sizes_), std::end(clique_sizes_)); }

    template <typename Clique, typename Graph>
    void clique(const Clique& c, const Graph& g)
    {
    	int size=0;
        // Iterate over the clique and count the vertices
        typename Clique::const_iterator i, end = c.end();
        for(i = c.begin(); i != end; ++i) {
            size++;
        }
        clique_sizes_.push_back(size);
    }
};

template <typename Graph>
class CliqueInducer : public BasicInducer<Graph>  {
public:
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	int clique_size;
	bool IsInducedK4(const Graph& g);
};

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
	// Use the Bron-Kerbosch algorithm to find all cliques
	bron_kerbosch_all_cliques(g, vis);

	int max_clique = vis.max_clique_size();
	if (max_clique >= subgraph_v){
		induced = true;
	}

	return induced;
}

// todo: this is similar to diamond inducer - combine the two somehow
template <typename Graph>
bool CliqueInducer<Graph>::IsInducedK4(const Graph& g){
	bool induced  = false;
	int i = num_vertices(g) - 1;

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

#endif /* UTILS_INDUCERS_CLIQUE_INDUCER_H_ */
