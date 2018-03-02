/*
 * basic_inducer.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: liz
 */

#include "../../utils/inducers/basic_inducer.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/copy.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/subgraph.hpp>

template <typename Graph>
bool BasicInducer<Graph>::IsInduced(const Graph& g, const Graph& subgraph){
	bool induced = false;
	int g_v = boost::num_vertices(g);
	int candidate_v = boost::num_vertices(subgraph);

	// if the graphs have the same number of vertices - they should be isomorphic
	if(g_v == candidate_v && boost::num_edges(g) == boost::num_edges(subgraph)){
		return IsIsomorphic(g, subgraph);
	}

	if(candidate_v <= g_v && boost::num_edges(subgraph) <= boost::num_edges(g))
	{
		int array[g_v];
		for( int i = 0; i < g_v; i++ ) {  array[i] = i; }

		IndexMap mapIndex;
		boost::associative_property_map<IndexMap> propmapIndex(mapIndex);

		// get all subsets without the last vertex
		vector< vector<int> > subsets = SubsetsOfSize(g_v - 1,candidate_v - 1);
		for (int i = 0; i < subsets.size() && !induced; i++)
		{
			vector<int> subset = subsets[i];
			// add the last vertex
			subset.push_back(g_v - 1);
			induced  = IsSubsetIsomorpic(g, subgraph, subset);
		}
	}

	return induced;
}

template <typename Graph>
bool BasicInducer<Graph>::IsSubsetIsomorpic(const Graph& g, const Graph& subgraph, vector<int> subset){
	Graph temp = this->InduceSubset(g, subset);
	bool induced = IsIsomorphic(temp, subgraph);
	return induced;
}

template <typename Graph>
Graph BasicInducer<Graph>::InduceSubset(const Graph& g, vector<int> subset){
	Graph temp;
	map<int,int> subset_map;
	//cout << "testing subset: ";
	int new_index = 0;
	for (auto &n : subset)
	{
	//cout << n;
		subset_map[n] = new_index;
		new_index++;
	}
	//cout << endl;
	temp.clear();
	// generate candidate
	for (int j =0; j < subset.size(); j++)
		boost::add_vertex(temp);

	for (typename Graph::edge_iterator ei = edges(g).first; ei != edges(g).second; ei++) {
		int inSubset = 0;
		for (auto &n : subset)
		{
			if (source(*ei, g) == n || target(*ei,g) == n)
				inSubset ++;
		}
		if (inSubset == 2){
			boost::add_edge(subset_map[source(*ei, g)], subset_map[target(*ei,g)], temp);
		}
	}

	return temp;
}

template <typename Graph>
bool BasicInducer<Graph>::IsIsomorphic(const Graph& g, const Graph& subgraph){
	int g_v = boost::num_vertices(g);
	int g_e = boost::num_edges(g);
	int candidate_v = boost::num_vertices(subgraph);
	int candidate_e =  boost::num_edges(subgraph);
	if (g_v == candidate_v && g_e == candidate_e){
		return boost::isomorphism(g, subgraph);
	}
	return false;
}

template <typename Graph>
vector< vector<int> > BasicInducer<Graph>::SubsetsOfSize(int n,  int k)
{
	vector<int> set;
	for ( int i = 0; i < n; i++ )
		  set.push_back(i);

	return SubsetsOfSize(set, k);
}

template <typename Graph>
vector< vector<int> > BasicInducer<Graph>::SubsetsOfSize(vector<int> set,  int k)
{
	vector< vector<int> > subset;
	vector<int> empty;
	subset.push_back( empty );

	for (int i = 0; i < set.size(); i++)
	{
		vector< vector<int> > subsetTemp = subset;

		for (int j = 0; j < subsetTemp.size(); j++)
			subsetTemp[j].push_back( set[i] );

		for (int j = 0; j < subsetTemp.size(); j++)
			subset.push_back( subsetTemp[j] );
	}

	vector< vector<int> > subset_filtered;
	for (int i = 0; i < subset.size(); i++){
		if (subset[i].size()  == k){
			subset_filtered.push_back(subset[i]);
		}
	}

	return subset_filtered;
}

template <typename Graph>
vector<int> BasicInducer<Graph>::GetNeighborhood(const Graph& g, int i){
	typename boost::graph_traits < Graph >::adjacency_iterator vi, vi_end;
	// get neighborhood
	vector<int> neighbors;

	boost::tie(vi, vi_end) = boost::adjacent_vertices(i, g);
	for (; vi != vi_end; vi++) {
		neighbors.push_back(*vi); //get(g, *vi)
	}
	return neighbors;
}
