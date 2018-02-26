/*
 * path_inducer.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: liz
 */

#include "path_inducer.h"
#include <boost/graph/adjacency_list.hpp>

using namespace boost;


template <typename Graph>
PathInducer<Graph>::PathInducer(int n) {
	path_size = n;
}

template <typename Graph>
bool PathInducer<Graph>::IsInduced(const Graph& g, const Graph& subgraph){
	bool induced = false;
	int subgraph_v = boost::num_vertices(subgraph);
	vector<vector<int>> paths = ListPaths(g, subgraph_v);
	if(paths.size() > 0)
	{
		induced = true;
	}
	return induced;
}

// todo: test properly
template <typename Graph>
vector<vector<int>> PathInducer<Graph>::ListPaths(const Graph& g, int n){

	vector<vector<int>> paths;
	typename graph_traits < Graph >::adjacency_iterator vi, vi_end;
	//typename boost::graph_traits<Graph>::adjacency_iterator adjacency_iterator;
	//adjacency_iterator vi, vi_end;
	int v = boost::num_vertices(g);

	if (n < 2)
		return paths;

	// base case - n is 2 - every edge is a path
	if (n == 2 ){
		for (typename Graph::edge_iterator ei = edges(g).first; ei != edges(g).second; ei++) {
			vector<int> path = {source(*ei, g), target(*ei, g)};
			paths.push_back(path);
		}
	}

	// base case - n is 3
	else if (n == 3){
		// for each vertex
		for(int i = 0; i <v; i++){
			// get neighborhood
			vector<int> neighbors;

			boost::tie(vi, vi_end) = boost::adjacent_vertices(i, g);
			for (; vi != vi_end; vi++) {
				neighbors.push_back(*vi); //get(g, *vi)
			}

			for (int j = 0; j < v; j++){
				// j not in i neighborhood
				auto it = find (neighbors.begin(), neighbors.end(), j);
				if (i != j && it == neighbors.end()){
					// consider k
					boost::tie(vi, vi_end) = boost::adjacent_vertices(j, g);
					for (; vi != vi_end; vi++) {
						if (boost::edge(i,*vi,g).second){
							vector<int> path;
							path.push_back(i);
							path.push_back(*vi);
							path.push_back(j);

							paths.push_back(path);
						}
					}
				}
			}
		}
	}

	// general case
	else{
		vector<vector<int>> smaller_paths = ListPaths(g, n-2);

		vector<int> current_path ;
		for (auto it = smaller_paths.begin(); it != smaller_paths.end(); ++it){
			current_path = *it;
			int front = current_path.front();
			int end = current_path.back();
			vector<int> front_candiates;
			vector<int> end_candiates;
			vector<int> path1;
			vector<int> path2;
			std::copy(current_path.begin() + 1, current_path.end(), back_inserter(path1));
			std::copy(current_path.begin(), current_path.end() -1, back_inserter(path2));

			// find vertices adjacent to front vertex
			boost::tie(vi, vi_end) = boost::adjacent_vertices(front, g);
			for (; vi != vi_end; vi++) {
				// check it's not adjacent to anything in the path
				if(CheckNonAdjacent(g, *vi, path1)){
					front_candiates.push_back(*vi);
				}
			}

			// find vertices adjacent to the end vertex
			boost::tie(vi, vi_end) = boost::adjacent_vertices(end, g);
			for (; vi != vi_end; vi++) {
				// check it's not adjacent to anything in the path
				if(CheckNonAdjacent(g, *vi, path2)){
					end_candiates.push_back(*vi);
				}

			}

			for (auto f_it = front_candiates.begin(); f_it != front_candiates.end(); ++f_it){
				for (auto e_it = end_candiates.begin(); e_it != end_candiates.end(); ++e_it){
					if (!boost::edge(*f_it,*e_it,g).second){
						vector<int> path;
						path.push_back(*f_it);
						path.insert(path.end(), current_path.begin(), current_path.end());
						path.push_back(*e_it);
						paths.push_back(path);
					}
				}
			}
		}

	}

	return paths;
}

template <typename Graph>
bool PathInducer<Graph>::CheckNonAdjacent(const Graph& g, int v, vector<int> list){
	typename graph_traits < Graph >::adjacency_iterator vi, vi_end;
	bool non_adjact = true;
	boost::tie(vi, vi_end) = boost::adjacent_vertices(v, g);
	for (; vi != vi_end; vi++) {
		auto it = find (list.begin(), list.end(), *vi);
		if (it != list.end()){
			non_adjact = false;
			break;
		}
	}
	return non_adjact;
}

