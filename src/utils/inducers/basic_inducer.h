/*
 * basic_inducer.h
 *
 *  Created on: Jan 27, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_BASIC_INDUCER_H_
#define UTILS_INDUCERS_BASIC_INDUCER_H_

#include <vector>
#include <map>

using namespace std;

template <typename Graph>
class BasicInducer {
typedef typename Graph::vertex_descriptor vertex_t;
typedef map<vertex_t, size_t> IndexMap;

public:
	virtual bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	vector< vector<int> > SubsetsOfSize(int n,  int k);
protected:
	bool IsIsomorphic(const Graph& g, const Graph& subgraph);
	bool IsSubsetInduced(const Graph& g, const Graph& subgraph, vector<int> subset);
	vector< vector<int> > SubsetsOfSize(vector<int> set,  int k);
};

#endif /* UTILS_INDUCERS_BASIC_INDUCER_H_ */
