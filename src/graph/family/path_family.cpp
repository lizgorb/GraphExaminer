/*
 * path_family.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: liz
 */

#include "path_family.h"
#include <utility>

template<typename Graph>
Graph PathFamily<Graph>::Create(int n) {
	typedef std::pair<int, int> E;

	E path_edges[n-1];

	for (int i = 0; i < n - 1; i++) {
		path_edges[i] = E(i, i+1);

	}
	Graph kn(&path_edges[0], &path_edges[0] + sizeof(path_edges) / sizeof(E), n);
	return kn;
}

