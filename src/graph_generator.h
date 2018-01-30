/*
 * GraphGenerator.h
 *
 *  Created on: 2017 M11 17
 *      Author: Liz
 */

#ifndef GRAPH_GENERATOR_H_
#define GRAPH_GENERATOR_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/isomorphism.hpp>

#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>

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
    vector<Graph> addVertex (const Graph& g);
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
    vector< vector<int> > getAllSubsets(int n);
    std::map<string,vector<Graph>> iso_graphs;
    string getSignature(const Graph& g);
    string Signature1(const Graph& g);
    string Signature2(const Graph& g);
    bool checkIsomorphic(const Graph& g);
};


#endif /* GRAPH_GENERATOR_H_ */
