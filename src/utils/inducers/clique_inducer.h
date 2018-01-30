/*
 * clique_inducer.h
 *
 *  Created on: Jan 28, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_CLIQUE_INDUCER_H_
#define UTILS_INDUCERS_CLIQUE_INDUCER_H_

#include "basic_inducer.h"

template <typename Graph>
class CliqueInducer : public BasicInducer<Graph>  {
public:
	CliqueInducer(int n);
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	int clique_size;
};

#endif /* UTILS_INDUCERS_CLIQUE_INDUCER_H_ */
