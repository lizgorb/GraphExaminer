/*
 * SubGraph.cpp
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#include "subgraph.h"

template <typename Graph>
bool Subgraph<Graph>::IsInduced(const Graph& g){
	return inducer->IsInduced(g, graph);

}
