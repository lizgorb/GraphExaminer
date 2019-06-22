/*
 * wheel_family.h
 *
 *  Created on: 2018 M01 25
 *      Author: Liz
 */

#ifndef GRAPH_FAMILY_WHEEL_FAMILY_H_
#define GRAPH_FAMILY_WHEEL_FAMILY_H_
#include "../../defs.hpp"

template<typename Graph>
class WheelFamily {
public:
	static Graph Create(int n) {

		// no wheels have less than 4 vertices
		if(n < 4){
			Graph g;
			return g;
		}

		E wn_edges[2 * (n - 1)];
		int i;
		int c = 0;

		for (i = 0; i < n - 1; i++) {
			if (i == n - 2) {
				wn_edges[c++] = E(i, 0);
			} else {
				wn_edges[c++] = E(i, i + 1);
			}

			wn_edges[c++] = E(i, n - 1);
		}

		Graph wn(&wn_edges[0], &wn_edges[0] + sizeof(wn_edges) / sizeof(E), n);
		return wn;
	}
};

#endif /* GRAPH_FAMILY_WHEEL_FAMILY_H_ */
