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
using namespace std;


template <typename Graph>
class InducedCheck {
	typedef typename Graph::vertex_descriptor vertex_t;
	typedef map<vertex_t, size_t> IndexMap;


  vector<Graph> checkGraphs;

  public:
	InducedCheck (){
		_CheckInducedCalls = 0;
		_CheckInducedSeconds = 0;
	};
	bool CheckInduced (const Graph& g);
	void AddInducedGraph(const Graph& g);
	void benchmark() {
	    	std::cout << "CheckInduced was called " << _CheckInducedCalls << " times and took " << _CheckInducedSeconds
	    			 << "(" << _CheckInducedSeconds/_CheckInducedCalls << " on avg)" << endl;
	    };
  private:
	bool is_isomorphic(const Graph& g, const Graph& candidate_g);
	vector< vector<int> > SubsetsOfSize(int n, int k);
	int _CheckInducedCalls;
	float _CheckInducedSeconds;
};


template <typename Graph>
bool InducedCheck<Graph>::CheckInduced (const Graph& g){
	_CheckInducedCalls++;
	clock_t t;
	t = clock();

	int g_v = boost::num_vertices(g);
	int array[g_v];
	for( int i = 0; i < g_v; i++ ) {  array[i] = i; }

	list<int> subsets;

	IndexMap mapIndex;
	boost::associative_property_map<IndexMap> propmapIndex(mapIndex);
	Graph temp;
	bool induced = false;
	for (auto &candidate_g : checkGraphs)
	{
		if (induced){
			break;
		}

		int candidate_v = boost::num_vertices(candidate_g);
		vector< vector<int> > subsets = SubsetsOfSize(g_v,candidate_v);

		for (int i = 0; i < subsets.size() && !induced; i++)
		{
			map<int,int> subset_map;
			vector<int> subset = subsets[i];
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

			induced = is_isomorphic(temp, candidate_g);
		}
	}

	_CheckInducedSeconds  += ((float)(clock() - t))/CLOCKS_PER_SEC;
	return induced;
}


template <typename Graph>
void InducedCheck<Graph>::AddInducedGraph (const Graph& g){
	checkGraphs.push_back(g);
}

template <typename Graph>
bool InducedCheck<Graph>::is_isomorphic(const Graph& g, const Graph& candidate_g){
	int g_v = boost::num_vertices(g);
	int g_e = boost::num_edges(g);
	int candidate_v = boost::num_vertices(candidate_g);
	int candidate_e =  boost::num_edges(candidate_g);
	if (g_v == candidate_v && g_e == candidate_e){
		return boost::isomorphism(g, candidate_g);
	}
	return false;
}

template <typename Graph>
vector< vector<int> > InducedCheck<Graph>::SubsetsOfSize(int n,  int k)
{
	vector<int> set;
	for ( int i = 0; i < n; i++ )
		  set.push_back(i);

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

