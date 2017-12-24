/*
 * GraphGenerator.cpp
 *
 *  Created on: 2017 M11 6
 *      Author: Liz
 */

#include "GraphGenerator.h"

template <typename Graph>
vector<Graph> GraphGenerator<Graph>::addVertex (const Graph& g)
{
	_addVertexCalls++;
	clock_t t;
	t = clock();

  //std::cout << "adding vertex\n";
  Graph temp = g;
  int nV = boost::num_vertices(temp);

  // generate all subsets
  vector<int> set;
  vector< vector<int> > subsets = getAllSubsets(nV);

  vector<Graph> graphs;

  Graph curr;
  // add edges
  for (int i = 0; i < subsets.size(); i++){
	  vector<int> subset = subsets[i];
	  int edges = subset.size();
	  if (edges > 0){
		  curr.clear();
		  copy_graph(temp, curr);
		  vertex_t v = boost::add_vertex(curr);

		  edge_t e; bool b;

		  int j = 0;
		  for (typename Graph::vertex_iterator vi = vertices(temp).first; vi != vertices(temp).second; vi++) {
			  if(std::find(subset.begin(), subset.end(), j) != subset.end()) {
				 vertex_t u = *vi;
				 boost::tie(e,b) = boost::add_edge(u,v,curr);
			  }
			  j++;
		   }

		 bool iso = false;
		 // check isomorphisms
		 for (auto &iso_g : iso_graphs[boost::num_edges(curr)])
		 {
			 if(boost::num_vertices(iso_g) == nV + 1){
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
				 if (iso){break;}
			 }
		 }
		 if (!iso){
			 iso_graphs[boost::num_edges(curr)].push_back(curr);
			 graphs.push_back(curr);
		 }
	  }
  }


  _addVertexSeconds += ((float)(clock() - t))/CLOCKS_PER_SEC;

  return graphs;
}

template <typename Graph>
vector< vector<int> > GraphGenerator<Graph>::getAllSubsets(int n)
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
    return subset;
}
