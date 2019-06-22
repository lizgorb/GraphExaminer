/*
 * InducedCheck.cpp
 *
 *  Created on: 2017 M11 6
 *      Author: Liz
 */

#ifndef INDUCED_CHECK_HPP_
#define INDUCED_CHECK_HPP_

#include <vector>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/copy.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/subgraph.hpp>
#include <time.h>
#include <iostream>
#include "subgraph.hpp"

using namespace std;


template <typename Graph>
class InducedCheck {
	typedef typename Graph::vertex_descriptor vertex_t;
	typedef map<vertex_t, size_t> IndexMap;


  vector<Subgraph<Graph>> checkGraphs;

  public:
	InducedCheck (){
		check_induced_calls_ = 0;
		check_induced_seconds_ = 0;
	};
	bool CheckInduced (const Graph& g);
	void AddInducedGraph(const Subgraph<Graph>& g);
	void benchmark() {
	    	std::cout << "CheckInduced was called " << check_induced_calls_ << " times and took " << check_induced_seconds_
	    			 << "(" << check_induced_seconds_/check_induced_calls_ << " on avg)" << endl;

	    	for (auto& t : subgraph_induced_seconds_)
	    	    std::cout << "	" << t.first << ": " << t.second << endl;
	    };
  private:
	int check_induced_calls_;
	float check_induced_seconds_;
	map<string, float> subgraph_induced_seconds_;
};


template <typename Graph>
bool InducedCheck<Graph>::CheckInduced (const Graph& g){
	#pragma omp critical
	{check_induced_calls_++;}
	clock_t t;
	clock_t t_subgraph;
	t = clock();

	bool induced = false;
	for (Subgraph<Graph> &candidate_g : checkGraphs)
	{
		if (induced){
			break;
		}
		t_subgraph = clock();
		induced = candidate_g.IsInduced(g);
		#pragma omp critical
		{subgraph_induced_seconds_[candidate_g.name] += ((float)(clock() - t_subgraph))/CLOCKS_PER_SEC;}

	}
	#pragma omp critical
	{check_induced_seconds_  += ((float)(clock() - t))/CLOCKS_PER_SEC;}
	return induced;
}


template <typename Graph>
void InducedCheck<Graph>::AddInducedGraph (const Subgraph<Graph>& g){
	checkGraphs.push_back(g);
	subgraph_induced_seconds_.insert( std::pair<string,int>(g.name, 0));
}

#endif /* INDUCED_CHECK_HPP_ */
