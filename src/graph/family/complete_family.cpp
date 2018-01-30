/*
 * CompleteClass.cpp
 *
 *  Created on: 2017 M11 6
 *      Author: Liz
 */

#include "complete_family.h"
#include <utility>

template<typename Graph>
Graph CompleteFamily<Graph>::Create(int n) {
	typedef std::pair<int, int> E;

	E kn_edges[(n * (n - 1))/2];
	int i, j;
	int c = 0;

	for (i = 0; i < n; i++) {
		for (j = i+1; j < n; j++) {
			kn_edges[c] = E(i, j);
			c++;
		}
	}
	Graph kn(&kn_edges[0], &kn_edges[0] + sizeof(kn_edges) / sizeof(E), n);
	return kn;
}

