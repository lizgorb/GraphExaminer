/*
 * GraphGenerator.cpp
 *
 *  Created on: 2017 M11 6
 *      Author: Liz
 */

#include "graph_generator.h"
#include <omp.h>

//todo: don't add dominated veretices
template<typename Graph>
vector<Graph> GraphGenerator<Graph>::AddVertex(const Graph& g) {
	_addVertexCalls++;
	clock_t t;
	t = clock();

	//std::cout << "adding vertex\n";

	int nV = boost::num_vertices(g);

	// generate all subsets
	vector<int> set;
	vector<vector<int> > subsets = getAllSubsets(nV);

	vector<Graph> graphs;

	// add edges
#pragma omp parallel for
	for (int i = 0; i < subsets.size(); i++) {

		//int tid = omp_get_thread_num();

		Graph temp = g;
		Graph curr;
		vector<int> subset = subsets[i];
		int edges = subset.size();
		if (edges > 0) {
			curr.clear();
			copy_graph(temp, curr);
			//int tid = omp_get_thread_num();
			vertex_t v = boost::add_vertex(curr);

			edge_t e;
			bool b;

			int j = 0;
			for (typename Graph::vertex_iterator vi = vertices(temp).first;
					vi != vertices(temp).second; vi++) {
				if (std::find(subset.begin(), subset.end(), j)
						!= subset.end()) {
					vertex_t u = *vi;
					boost::tie(e, b) = boost::add_edge(u, v, curr);
				}
				j++;
			}

			bool isomorphic;

			isomorphic = checkIsomorphic(curr);

			if (!isomorphic) {
#pragma omp critical
				{
					graphs.push_back(curr);
				}
			}
		}
	}

	_addVertexSeconds += ((float) (clock() - t)) / CLOCKS_PER_SEC;

	return graphs;
}

template<typename Graph>
vector<vector<int> > GraphGenerator<Graph>::getAllSubsets(int n) {
	vector<int> set;
	for (int i = 0; i < n; i++)
		set.push_back(i);

	vector<vector<int> > subset;
	vector<int> empty;
	subset.push_back(empty);

	for (int i = 0; i < set.size(); i++) {
		vector<vector<int> > subsetTemp = subset;

		for (int j = 0; j < subsetTemp.size(); j++)
			subsetTemp[j].push_back(set[i]);

		for (int j = 0; j < subsetTemp.size(); j++)
			subset.push_back(subsetTemp[j]);
	}
	return subset;
}

template<typename Graph>
string GraphGenerator<Graph>::getSignature(const Graph& g) {
	// todo:  Find a way to stick this property to graph objects
	// it can then be more efficiently constructed
	clock_t t = clock();
	string sig = Signature3(g);
	_getSignatureSeconds += ((float) (clock() - t)) / CLOCKS_PER_SEC;
	return sig;
}

template<typename Graph>
string GraphGenerator<Graph>::Signature1(const Graph& g) {
	int nV = boost::num_vertices(g);
	int v_degrees[nV];
	string v_neighbors[nV];
	vector<int> adj;
	std::pair<adjacency_iterator, adjacency_iterator> n;
	for (int i = 0; i < nV; i++) {
		v_degrees[i] = boost::degree(i, g);
	}

	sort(v_degrees, v_degrees + nV);
	stringstream sig("");
	for (int i = 0; i < nV; i++)
		sig << v_degrees[i];
	return sig.str();
}

template<typename Graph>
string GraphGenerator<Graph>::Signature2(const Graph& g) {
	int nV = boost::num_vertices(g);
	int v_degrees[nV];
	for (int i = 0; i < nV; i++) {
		v_degrees[i] = boost::degree(i, g);
	}

	std::pair<adjacency_iterator, adjacency_iterator> n;
	int adj[nV];
	for (int i = 0; i < nV; i++) {
		n = adjacent_vertices(i, g);

		int count = 0;
		for (; n.first != n.second; ++n.first) {
			count += v_degrees[*n.first];
		}
		adj[i] = count;
	}

	sort(adj, adj + nV);

	stringstream sig("");
	for (int i = 0; i < nV; i++)
		sig << adj[i] << ",";
	//sig << "(" << v_neighbors[i] << ")";
	return sig.str();
}

template<typename Graph>
string GraphGenerator<Graph>::Signature3(const Graph& g) {
	int nV = boost::num_vertices(g);
	int v_degrees[nV];
	for (int i = 0; i < nV; i++) {
		v_degrees[i] = boost::degree(i, g);
	}

	std::pair<adjacency_iterator, adjacency_iterator> n;
	std::pair<int, int> adj[nV];
	for (int i = 0; i < nV; i++) {
		n = adjacent_vertices(i, g);

		int count = 0;
		for (; n.first != n.second; ++n.first) {
			count += v_degrees[*n.first];
		}
		adj[i] = std::pair<int, int>(v_degrees[i], count);
	}

	sort(adj, adj + nV);

	stringstream sig("");
	for (int i = 0; i < nV; i++)
		sig << adj[i].first << "-" << adj[i].second << ",";

	return sig.str();
}

template<typename Graph>
bool GraphGenerator<Graph>::checkIsomorphic(const Graph& curr) {
	clock_t t = clock();

	bool iso = false;
	string key = "";
#pragma omp critical
	{
		key = getSignature(curr);
	}

	//std::cout << "key:"<< key<< endl;
	vector<Graph> key_graphs ;
#pragma omp critical
	{
		key_graphs = iso_graphs[key];
	}
	// check isomorphisms
	for (auto &iso_g : key_graphs) {
#pragma omp critical
		{
			_isoChecks++;
		}
		/*std::cout << "checking if: ";
		 for (typename Graph::edge_iterator ei = boost::edges(curr).first; ei != boost::edges(curr).second; ei++) {
		 std::cout << "(" << source(*ei, curr) << "-" << target(*ei,curr) <<  ")";
		 }
		 std::cout << " is isomoriphic to: ";
		 for (typename Graph::edge_iterator ei = boost::edges(iso_g).first; ei != boost::edges(iso_g).second; ei++) {
		 std::cout << "(" << source(*ei, iso_g) << "-" << target(*ei,iso_g) <<  ")";
		 }
		 std::cout << endl;*/
		iso = boost::isomorphism(curr, iso_g);
		if (iso) {
			break;

		}
	}
#pragma omp critical
	{
		if (!iso) {
			iso_graphs[key].push_back(curr);
		}
	}
#pragma omp critical
	{
		_isoCheckSeconds += ((float) (clock() - t)) / CLOCKS_PER_SEC;
	}

	return iso;
}
