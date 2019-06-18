/*
 * GraphGenerator.h
 *
 *  Created on: 2017 M11 17
 *      Author: Liz
 */

#ifndef GRAPH_GENERATOR_HPP_
#define GRAPH_GENERATOR_HPP_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/connected_components.hpp>

#include <omp.h>
#include <ctime>
#include <vector>
#include <algorithm> // std::random_shuffle
#include <iostream>
#include <time.h>

#include "common_utils.hpp"
using namespace std;


template <typename Graph>
class GraphGenerator {

	typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;
	typedef typename boost::graph_traits<Graph>::edge_descriptor edge_t;
	typedef typename boost::graph_traits<Graph>::adjacency_iterator adjacency_iterator;

  public:
    GraphGenerator (){
    	_addVertexCalls = 0;
    	_addVertexSeconds = 0;
    	_isoChecks = _isoCheckSeconds = _getSignatureSeconds = 0;
    };
    vector<Graph> AddVertex(const Graph& g);
    static Graph RandomGraph(int n);
    void benchmark() {
    	std::cout << "addVertex was called " << _addVertexCalls << " times and took " << _addVertexSeconds
    			 << "(" << _addVertexSeconds/_addVertexCalls << " on avg)" << endl;
    	std::cout << "	isoCheck was made " << _isoChecks << " times and took " << _isoCheckSeconds  << endl;
    	std::cout << "	signature computation took " << _getSignatureSeconds << " seconds " <<  endl;
    	int unique = 0;
    	for (auto const& x : iso_graphs)
    	{
    		unique += x.second.size();
    	}


    	std::cout << "	iso_graphs has " << iso_graphs.size() << " keys and " << unique << " values"  << endl;
    };
  private:
    int _addVertexCalls;
    float _addVertexSeconds;
    int _isoChecks;
    float _isoCheckSeconds;
    float _getSignatureSeconds;
    std::map<string,vector<Graph>> iso_graphs;
    string getSignature(const Graph& g);
    string Signature1(const Graph& g);
    string Signature2(const Graph& g);
    string Signature3(const Graph& g);
    bool checkIsomorphic(const Graph& g);
};


template<typename Graph>
Graph GraphGenerator<Graph>::RandomGraph(int n){
	typedef std::pair<int, int> E;

	int max_edges = (n * (n-1)) / 2;
	int min_edges = floor (1.5* n);
	int num_edges = (rand() % (max_edges - min_edges)) + min_edges;

	// create all subsets of size 2
	vector<vector<int>> all_subsets = CommonUtils::SubsetsOfSize(n, 2);

	// choose random num_edges from subsets
	std::random_shuffle ( all_subsets.begin(), all_subsets.end() );

	Graph random (n);
	int edges_count = 0;

	while (edges_count < num_edges){
		boost::add_edge(all_subsets[edges_count][0], all_subsets[edges_count][1] ,random);
		edges_count++;

	}

	// if the graph is disconnected add more edges until it becomes connected
	std::vector<int> component(n);
	size_t num_components = boost::connected_components (random, &component[0]);
	while (num_components > 1){
		boost::add_edge(all_subsets[edges_count][0], all_subsets[edges_count][1] ,random);
		edges_count++;
		num_components = boost::connected_components (random, &component[0]);
	}

	return random;
}

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
	vector<vector<int>> subsets = CommonUtils::AllSubsets(nV);

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
string GraphGenerator<Graph>::getSignature(const Graph& g) {
	// todo:  Find a way to stick this property to graph objects
	// it can then be more efficiently constructed
	clock_t t = clock();
	string sig;
	sig = Signature3(g);
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

#endif /* GRAPH_GENERATOR_HPP_ */
