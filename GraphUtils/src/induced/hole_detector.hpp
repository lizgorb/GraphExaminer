/*
 * HoleDetector.h
 *
 *  Created on: 2017 M12 15
 *      Author: Liz
 */

#ifndef HOLEDETECTOR_H_
#define HOLEDETECTOR_H_

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

using namespace boost;
using namespace std;

template <typename OutputStream>
class hole_checker
{
private:
		OutputStream& os = cout;
		vector<vector<int>>& _even_holes;

public:
    hole_checker(vector<vector<int>>& even_holes) : _even_holes(even_holes)  { }
	int num_even_holes(){return _even_holes.size();}
	void add_even_hole(vector<int> x){_even_holes.push_back(x);}

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

        if(vertices.size() % 2 == 0){
        	bool has_cord = false;

			std::pair<adjacency_iterator,adjacency_iterator> neighbors;

			for(i = p.begin(); i != end && !has_cord; ++i) {
				int edges = 0;
				neighbors = adjacent_vertices(*i, g);
				for(; neighbors.first != neighbors.second && !has_cord; ++neighbors.first)
				{
					if(std::find(vertices.begin(), vertices.end(), get(indices, *neighbors.first)) != vertices.end()) {
					    edges++;
					}

					if(edges > 2){
						has_cord = true;
					}
				}
			}

			if(!has_cord){
				add_even_hole(vertices);
				/*os << "found even hole ";
				for(i = p.begin(); i != end; ++i) {
				    os << get(indices, *i) << " ";
				}
			   os << endl;*/
			}
        }
    }

    bool has_even_hole(){
       return num_even_holes() == 0 ? false : true;
    }

};

template <typename OutputStream>
class hole_collector
{
private:
		OutputStream& os = cout;
		vector<vector<int>>& _holes;

public:
	hole_collector(vector<vector<int>>& holes) : _holes(holes)  { }
	int num_holes(){return _holes.size();}
	void add_hole(vector<int> x){_holes.push_back(x);}

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


		bool has_cord = false;

		std::pair<adjacency_iterator,adjacency_iterator> neighbors;

		for(i = p.begin(); i != end && !has_cord; ++i) {
			int edges = 0;
			neighbors = adjacent_vertices(*i, g);
			for(; neighbors.first != neighbors.second && !has_cord; ++neighbors.first)
			{
				if(std::find(vertices.begin(), vertices.end(), get(indices, *neighbors.first)) != vertices.end()) {
					edges++;
				}

				if(edges > 2){
					has_cord = true;
				}
			}
		}

		if(!has_cord && vertices.size() > 3){
			add_hole(vertices);
		}
    }
};

template <typename Graph>
class HoleDetector {
	typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;
	typedef boost::undirected_graph<boost::no_property> UDGraph;

	public:
		HoleDetector (){
			_CheckEvenHoleCalls = 0;
			_CheckEvenHoleSeconds = 0;
	    };
		bool CheckEvenHole (const Graph& g);
		vector<vector<int>> GetAllHoles(const Graph& g);
		 void benchmark() {
		    	std::cout << "CheckEvenHole was called " << _CheckEvenHoleCalls << " times and took " << _CheckEvenHoleSeconds
		    			 << "(" << _CheckEvenHoleSeconds/_CheckEvenHoleCalls << " on avg)" << endl;
		    };
	private:
		 int _CheckEvenHoleCalls;
		 float _CheckEvenHoleSeconds;
};


template <typename Graph>
bool HoleDetector<Graph>::CheckEvenHole (const Graph& g){
	#pragma omp critical
	{_CheckEvenHoleCalls++;}
	clock_t t;
	t = clock();

	 // convert to undirected graph
	 UDGraph udg;

	 int nV = num_vertices(g);

	 int i;
	 graph_traits<UDGraph>::vertex_descriptor vetices[nV];
	 for(i = 0; i < nV; i++){
		 vetices[i] = udg.add_vertex();
	 }
	 for (typename Graph::edge_iterator ei = edges(g).first; ei != edges(g).second; ei++) {
		 udg.add_edge(vetices[source(*ei, g)], vetices[target(*ei,g)]);
	 }

	 vector<vector<int>> even_holes;
	 hole_checker<ostream> vis(even_holes);
	 vis.has_even_hole();
	 // Use the Tiernan algorithm to visit all cycles
	 tiernan_all_cycles(udg, vis);

	#pragma omp critical
	 { _CheckEvenHoleSeconds += ((float)(clock() - t))/CLOCKS_PER_SEC;}
	 return vis.has_even_hole();
}

template <typename Graph>
vector<vector<int>> HoleDetector<Graph>::GetAllHoles(const Graph& g){
	vector<vector<int>> holes;

	 // convert to undirected graph
	 UDGraph udg;

	 int nV = num_vertices(g);

	 int i;
	 graph_traits<UDGraph>::vertex_descriptor vetices[nV];
	 for(i = 0; i < nV; i++){
		 vetices[i] = udg.add_vertex();
	 }
	 for (typename Graph::edge_iterator ei = edges(g).first; ei != edges(g).second; ei++) {
		 udg.add_edge(vetices[source(*ei, g)], vetices[target(*ei,g)]);
	 }
     hole_collector<ostream> vis(holes);
	 // Use the Tiernan algorithm to visit all cycles
	 tiernan_all_cycles(udg, vis);

	 return holes;
}

#endif /* HOLEDETECTOR_H_ */
