/*
 * path_family.h
 *
 *  Created on: Feb 25, 2018
 *      Author: liz
 */

#ifndef GRAPH_FAMILY_PATH_FAMILY_H_
#define GRAPH_FAMILY_PATH_FAMILY_H_
#include "../../defs.hpp"

template<typename Graph>
class PathFamily {
public:
	static Graph Create(int n) {

		E path_edges[n-1];

		for (int i = 0; i < n - 1; i++) {
			path_edges[i] = E(i, i+1);

		}
		Graph kn(&path_edges[0], &path_edges[0] + sizeof(path_edges) / sizeof(E), n);
		return kn;
	}
};

#endif /* GRAPH_FAMILY_PATH_FAMILY_H_ */
