/*
 * Cycle inducer.h
 *
 *  Created on:
 *      Author: Liz
 */

#ifndef UTILS_INDUCERS_CYCLE_INDUCER_H_
#define UTILS_INDUCERS_CYCLE_INDUCER_H_

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/tiernan_all_cycles.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/visitors.hpp>
#include <vector>
#include <time.h>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>

#include "basic_inducer.hpp"

using namespace boost;
using namespace std;



template <typename OutputStream>
class cycle_collector
{
private:
		int min_deg = 3;
		bool collect_min_degree = false;
		OutputStream& os = cout;
		vector<vector<int>>& _cycles;

public:
	cycle_collector(vector<vector<int>>& cycles) : _cycles(cycles)  { }
	int num_cycles(){return _cycles.size();}
	int max_length_cycle(){
		int max_length  = 0;
		if (_cycles.size() > 0)
		{
			for(auto cycle: _cycles)
			{
				if(cycle.size() > max_length){
					max_length = cycle.size();
				}
			}
		}
		return max_length;
	}
	void add_cycle(vector<int> x){_cycles.push_back(x);}

    template <typename Path, typename Graph>
    void cycle(const Path& p, const Graph& g)
    {
        // Get the property map containing the vertex indices
        typedef typename property_map<Graph, vertex_index_t>::const_type IndexMap;
        typedef typename graph_traits<Graph>::adjacency_iterator  adjacency_iterator;

        IndexMap indices = get(vertex_index, g);

        vector<int> vertices;

        // Iterate over path printing each vertex that forms the cycle.
        typename Path::const_iterator i, end = p.end();
        for(i = p.begin(); i != end; ++i) {
            vertices.push_back(get(indices, *i));
        }

        if(!collect_min_degree)
        {
        	add_cycle(vertices);
        }
        else{

			bool has_min_deg = true;

			std::pair<adjacency_iterator,adjacency_iterator> neighbors;

			for(i = p.begin(); i != end && has_min_deg; ++i) {
				int edges = 0;
				neighbors = adjacent_vertices(*i, g);
				for(; neighbors.first != neighbors.second; ++neighbors.first)
				{
					if(std::find(vertices.begin(), vertices.end(), get(indices, *neighbors.first)) != vertices.end()) {
						edges++;
					}
				}
				if(edges < min_deg){
					has_min_deg = false;
				}
			}
			if(has_min_deg){
				add_cycle(vertices);
			}
        }
    }
    bool has_cycle(){
	   return num_cycles() == 0 ? false : true;
	}
};

template <typename Graph>
class CycleInducer : public BasicInducer<Graph> {
	typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;
	typedef boost::undirected_graph<boost::no_property> UDGraph;

	public:
		CycleInducer(){

	    };
		vector<Graph> GetAllCycles(const Graph& g);
		int MaxCycle(const Graph& g);

	private:
};

#endif /* UTILS_INDUCERS_CYCLE_INDUCER_H_ */
