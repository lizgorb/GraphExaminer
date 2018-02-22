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

using namespace std;

typedef std::pair <int, int> E;

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
public:
		static bool Contains ( string name );
		static CommonGraph Get( string name );
};


template< typename Graph>
class CommonGraphsCreator {
	public:
		static Graph Create( string name ){
			CommonGraph cg =  CommonGraphs::Get(name);
			Graph g (cg.edges, cg.edges + cg.num_edges, cg.num_vertices);
			return g;
		}
};


#endif /* GRAPH_COMMON_GRAPHS_H_ */
