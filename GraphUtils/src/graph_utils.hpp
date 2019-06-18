/*
 * graph_utils.h
 *
 *  Created on: Jun 13, 2018
 *      Author: liz
 */

#ifndef GRAPH_UTILS_HPP_
#define GRAPH_UTILS_HPP_
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string.hpp>

#include "common_utils.hpp"

using namespace std;

class GraphUtils {
public:

	template<typename Graph>
	static void PrintGraph(const Graph& g, std::ostream& os = std::cout) {
		for (typename Graph::edge_iterator ei = edges(g).first;
				ei != edges(g).second; ei++) {
			os << source(*ei, g) << "--" << target(*ei, g) << " ";
		}
		os << endl;
	}

	template<typename Graph>
	static Graph FromString(const string& graph_s, Graph& type) {
		int num_vertices = 0;
		vector<string> edges_s;

		boost::split(edges_s, graph_s, boost::is_any_of(" "));
		vector<pair<int, int>> edges;

		for (auto &edge_s : edges_s) {
			if (edge_s != "") {
				vector<string> nodes;
				boost::split(nodes, edge_s, boost::is_any_of("-"));
				int x = stoi(nodes[0]);
				int y = stoi(nodes[2]);
				std::pair<int, int> edge;
				edge.first = x;
				edge.second = y;

				edges.push_back(edge);

				int max_vertex = max(x, y);
				num_vertices = max(num_vertices, max_vertex);

			}
		}

		Graph g(num_vertices);

		for (auto &edge : edges) {
			boost::add_edge(edge.first, edge.second, g);
		}
		type = g;
		return g;
	}

	template<typename Graph>
	static bool IsSimplicial(const Graph& g, int vertex) {

		bool simplicial = true;
		// check if the neighborhood is a clique
		vector<int> neighbors;
		typename Graph::adjacency_iterator vi, vi_end;
		for (boost::tie(vi, vi_end) = adjacent_vertices(vertex, g);
				vi != vi_end; ++vi) {
			neighbors.push_back(*vi);
		}

		vector<vector<int>> clique_edges = CommonUtils::SubsetsOfSize(neighbors, 2);
			//	GraphUtils::GUSubsetsOfSize(neighbors, 2);

		for (vector<int> ce : clique_edges) {
			if (!boost::edge(ce[0], ce[1], g).second) {
				simplicial = false;
			}
		}
		return simplicial;
	}
};

#endif /* GRAPH_UTILS_HPP_ */
