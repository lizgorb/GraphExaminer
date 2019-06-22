/*
 * CommonGraphs.h
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#ifndef GRAPH_COMMON_GRAPHS_H_
#define GRAPH_COMMON_GRAPHS_H_

#include <string>
#include <map>
#include "../defs.hpp"

using namespace std;

//typedef std::pair <int, int> E;

struct CommonGraph {
	int num_vertices;
	int num_edges;
	E* edges;
};

class CommonGraphs {
private:
		static map<string, CommonGraph> common_graphs_;
		static CommonGraph bowtie;
		static E bowtie_edges[];
		static CommonGraph diamond;
		static E diamond_edges[];
		static E kite_edges[];
		static CommonGraph kite;
		static E bicycle_edges[];
		static CommonGraph bicycle;

public:
		static bool Contains ( string name );
		static CommonGraph Get( string name );
};


template< typename Graph>
class CommonGraphsCreator {
	public:
		static Graph Create( string name ){
			if(CommonGraphs::Contains(name))
			{
				CommonGraph cg =  CommonGraphs::Get(name);
				Graph g (cg.edges, cg.edges + cg.num_edges, cg.num_vertices);
				return g;
			}
			else{
				Graph g;
				return g;
			}
		}
};


E CommonGraphs::diamond_edges[] = { E (0, 1), E (0, 2), E (1, 2), E(1,3), E (2,3) };
CommonGraph CommonGraphs::diamond = {4, 5, CommonGraphs::diamond_edges};
E CommonGraphs::bowtie_edges[] = { E (0, 1), E (0, 2), E (1, 2), E (2,3), E (2, 4), E (3, 4) };
CommonGraph CommonGraphs::bowtie = {5, 6, CommonGraphs::bowtie_edges};
E CommonGraphs::kite_edges[] = { E (0, 1), E (0, 2), E (1, 2), E(1,3), E (2,3), E(3, 4) };
CommonGraph CommonGraphs::kite = {5, 6, CommonGraphs::kite_edges};
E CommonGraphs::bicycle_edges[] = { E(0,1),E(0,2),E(1,2),E(2,3),E(3,4),E(3,5),E(4,5) };
CommonGraph CommonGraphs::bicycle = {6, 7, CommonGraphs::bicycle_edges};

map<string, CommonGraph> CommonGraphs::common_graphs_ = {
    { "bowtie",   CommonGraphs::bowtie },
	{ "diamond", CommonGraphs::diamond },
	{ "kite",   CommonGraphs::kite },
	{ "bicycle",   CommonGraphs::bicycle }
};

bool CommonGraphs::Contains ( string name ){
	bool c = false;
	if (common_graphs_.count(name)){
		c = true;
	}
	return c;
}

CommonGraph CommonGraphs::Get ( string name ){
	return common_graphs_[name];
}


#endif /* GRAPH_COMMON_GRAPHS_H_ */
