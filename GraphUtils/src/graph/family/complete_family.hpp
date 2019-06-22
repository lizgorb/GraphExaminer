/*
 * complete_family.h
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#ifndef GRAPH_FAMILY_COMPLETE_FAMILY_H_
#define GRAPH_FAMILY_COMPLETE_FAMILY_H_
#include "../../defs.hpp"

template<typename Graph>
class CompleteFamily {
public:
	static Graph Create(int n){

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
};

#endif /* GRAPH_FAMILY_COMPLETE_FAMILY_H_ */
