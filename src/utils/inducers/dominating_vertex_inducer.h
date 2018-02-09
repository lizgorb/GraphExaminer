/*
 * max_vertex_inducer.h
 *
 *  Created on: Jan 28, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_DOMINATING_VERTEX_INDUCER_H_
#define UTILS_INDUCERS_DOMINATING_VERTEX_INDUCER_H_

#include "basic_inducer.h"

//todo: Extend to dominating clique

template <typename Graph>
class DominatingVertexInducer: public BasicInducer<Graph> {
public:
	DominatingVertexInducer(int n);
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	int max_degree;
};

#endif /* UTILS_INDUCERS_DOMINATING_VERTEX_INDUCER_H_ */
