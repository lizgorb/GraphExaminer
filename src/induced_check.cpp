/*
 * InducedCheck.cpp
 *
 *  Created on: 2017 M11 6
 *      Author: Liz
 */

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
#include "graph/subgraph.cpp"

using namespace std;


template <typename Graph>
class InducedCheck {
	typedef typename Graph::vertex_descriptor vertex_t;
	typedef map<vertex_t, size_t> IndexMap;


  vector<Subgraph<Graph>> checkGraphs;

  public:
	InducedCheck (){
		_CheckInducedCalls = 0;
		_CheckInducedSeconds = 0;
	};
	bool CheckInduced (const Graph& g);
	void AddInducedGraph(const Subgraph<Graph>& g);
	void benchmark() {
	    	std::cout << "CheckInduced was called " << _CheckInducedCalls << " times and took " << _CheckInducedSeconds
	    			 << "(" << _CheckInducedSeconds/_CheckInducedCalls << " on avg)" << endl;
	    };
  private:
	int _CheckInducedCalls;
	float _CheckInducedSeconds;
};


template <typename Graph>
bool InducedCheck<Graph>::CheckInduced (const Graph& g){
	_CheckInducedCalls++;
	clock_t t;
	t = clock();

	bool induced = false;
	for (Subgraph<Graph> &candidate_g : checkGraphs)
	{
		if (induced){
			break;
		}
		induced = candidate_g.IsInduced(g);
	}

	_CheckInducedSeconds  += ((float)(clock() - t))/CLOCKS_PER_SEC;
	return induced;
}


template <typename Graph>
void InducedCheck<Graph>::AddInducedGraph (const Subgraph<Graph>& g){
	checkGraphs.push_back(g);
}
