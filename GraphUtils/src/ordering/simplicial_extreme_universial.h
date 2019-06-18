/*
 * simplicial_extreme_universial.h
 *
 *  Created on: Nov 17, 2018
 *      Author: liz
 */

#ifndef SRC_SEARCH_SIMPLICIAL_EXTREME_UNIVERSIAL_H_
#define SRC_SEARCH_SIMPLICIAL_EXTREME_UNIVERSIAL_H_
#include <vector>

using namespace std;

class SimplicialExtremeUniversial {
public:
	SimplicialExtremeUniversial();
	template <typename Graph>
	static vector<int> GenerateOrder(const Graph &g);

	template <typename Graph>
	static bool VerifyOrder(const Graph &g, vector<int> order);
};

#endif /* SRC_SEARCH_SIMPLICIAL_EXTREME_UNIVERSIAL_H_ */
