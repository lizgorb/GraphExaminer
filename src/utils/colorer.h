/*
 * Colorer.h
 *
 *  Created on: 2017 M12 17
 *      Author: Liz
 */

#ifndef COLORER_H_
#define COLORER_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>

using namespace std;
using namespace boost;

template <typename Graph>
class Colorer {

    typedef typename graph_traits<Graph>::adjacency_iterator  adjacency_iterator;
    typedef typename property_map<Graph, vertex_index_t>::const_type IndexMap;
public:
    Colorer (){
		_ThreeColorCalls = 0;
		_ThreeColorSeconds = 0;
	};
	bool ThreeColor(const Graph& g,  map<int, int> color_map);
	 void benchmark() {
		std::cout << "ThreeColor was called " << _ThreeColorCalls << " times and took " << _ThreeColorSeconds
				 << "(" << _ThreeColorSeconds/_ThreeColorCalls << " on avg)" << endl;
	};
private:
	vector<int> possibleColors(const Graph& g, int v, map<int, int> color_map);
	 int _ThreeColorCalls;
	 float _ThreeColorSeconds;
};

#endif /* COLORER_H_ */
