/*
 * mcs.h
 *
 *  Created on: Jun 13, 2018
 *      Author: liz
 */

#ifndef SEARCH_MCS_H_
#define SEARCH_MCS_H_

#include <vector>
using namespace std;

class MCS {
public:
	template <typename Graph>
	static vector<int> GenerateOrder(const Graph &g);

	template <typename Graph>
	static std::vector<int> GenerateOrder(const Graph &g, int start_index);

	template <typename Graph>
	static std::vector<std::vector<int>> GenerateAllOrders(const Graph &g);

	template <typename Graph>
	static std::vector<vector<int>> GenerateAllSimplicialExtremeOrders(const Graph &g);

};

#endif /* SEARCH_MCS_H_ */
