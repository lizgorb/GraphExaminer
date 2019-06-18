/*
 * diamond_inducer.h
 *
 *  Created on: Mar 2, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_DIAMOND_INDUCER_H_
#define UTILS_INDUCERS_DIAMOND_INDUCER_H_

#include "basic_inducer.hpp"

template <typename Graph>
class DiamondInducer : public BasicInducer<Graph>  {
public:
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	bool IsCentralVertex(const Graph& g, int i);
};

#endif /* UTILS_INDUCERS_DIAMOND_INDUCER_H_ */
