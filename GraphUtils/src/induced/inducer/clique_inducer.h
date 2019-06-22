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
#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include <boost/graph/depth_first_search.hpp>
#include "basic_inducer.hpp"

using namespace boost;

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
        // Iterate over the clique and print each vertex within it.
        typename Clique::const_iterator i, end = c.end();
        for(i = c.begin(); i != end; ++i) {
           // std::cout << *i << " ";
            size++;
        }
       // std::cout << endl;
        clique_sizes_.push_back(size);

    }
};

template <typename Graph>
class CliqueInducer : public BasicInducer<Graph>  {
public:
	CliqueInducer(int n);
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	int clique_size;
	bool IsInducedK4(const Graph& g);
};

#endif /* UTILS_INDUCERS_CLIQUE_INDUCER_H_ */
