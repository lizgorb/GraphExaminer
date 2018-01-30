/*
 * SubGraphFactory.h
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#ifndef GRAPH_SUBGRAPH_FACTORY_H_
#define GRAPH_SUBGRAPH_FACTORY_H_

#include <string>
#include "subgraph.h"

using namespace std;

template <typename Graph>
class SubgraphFactory {
public:
	static Subgraph<Graph> Create( string name );
};

#endif /* GRAPH_SUBGRAPH_FACTORY_H_ */
