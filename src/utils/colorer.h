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
		debug = false;
		setColorLimit(3);
	};
    bool ThreeColor(const Graph& g,  map<int, int> &color_map);
	bool ThreeColor(const Graph& g,  map<int, int> color_map, string token,
			map<int,int> &_index_mapping, map<int,int> &_final_color_map );
	void benchmark() {
		std::cout << "ThreeColor was called " << _ThreeColorCalls << " times and took " << _ThreeColorSeconds
				 << "(" << _ThreeColorSeconds/_ThreeColorCalls << " on avg)" << endl;
	};

	void setColorLimit(int color_limit){
		_ColorLimit = color_limit;

		for(int i=0; i< _ColorLimit; i++){
			_AllColors.push_back(i+1);
		}
	};
private:
	 vector<int> possibleColors(const Graph& g, int v, map<int, int> color_map);
	 bool ColorWithSameColor(const Graph& g, map<int, int> color_map, string token,
			int x, int y, vector<int>  x_colors, vector<int>  y_colors,
			map<int,int> &_index_mapping, map<int,int> &_final_color_map);
	 bool ColorWithDifferentColor(const Graph& g, map<int, int> color_map, string token,
			int x, int y, vector<int>  x_colors, vector<int>  y_colors,
			map<int,int> &_index_mapping, map<int,int> &_final_color_map);

	 void initIndexMap(int length, map<int,int> &_index_mapping);
	 void initColorMap(int length, map<int,int> &_final_color_map);

	 int _ThreeColorCalls;
	 float _ThreeColorSeconds;
	 int _ColorLimit;
	 vector<int> _AllColors;
	 bool debug;
};

#endif /* COLORER_H_ */
