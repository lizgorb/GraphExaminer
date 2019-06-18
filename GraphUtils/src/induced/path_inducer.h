/*
 * path_inducer.h
 *
 *  Created on: Feb 25, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_PATH_INDUCER_H_
#define UTILS_INDUCERS_PATH_INDUCER_H_

#include "basic_inducer.hpp"

template <typename Graph>
class PathInducer : public BasicInducer<Graph>  {
public:
	PathInducer(int n);
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	int path_size;
	vector<vector<int>> ListPaths(const Graph& g, int n);
	bool CheckNonAdjacent(const Graph& g, int v, vector<int> list);
};

#endif /* UTILS_INDUCERS_PATH_INDUCER_H_ */
