/*
 * GraphGenerator.h
 *
 *  Created on: 2017 M11 17
 *      Author: Liz
 */

#ifndef GRAPHGENERATOR_H_
#define GRAPHGENERATOR_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/isomorphism.hpp>

#include <vector>
#include <iostream>
#include <time.h>

using namespace std;


template <typename Graph>
class GraphGenerator {

	typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;
	typedef typename boost::graph_traits<Graph>::edge_descriptor edge_t;

  public:
    GraphGenerator (){
    	_addVertexCalls = 0;
    	_addVertexSeconds = 0;
    };
    vector<Graph> addVertex (const Graph& g);
    void benchmark() {
    	std::cout << "addVertex was called " << _addVertexCalls << " times and took " << _addVertexSeconds
    			 << "(" << _addVertexSeconds/_addVertexCalls << " on avg)" << endl;
    };
  private:
    int _addVertexCalls;
    float _addVertexSeconds;
    vector< vector<int> > getAllSubsets(int n);
    std::map<int,vector<Graph>> iso_graphs;
};


#endif /* GRAPHGENERATOR_H_ */
