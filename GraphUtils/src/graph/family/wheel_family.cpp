/*
 * wheel_family.cpp
 *
 *  Created on: 2018 M01 25
 *      Author: Liz
 */

#include "wheel_family.h"
#include <utility>

template<typename Graph>
Graph WheelFamily<Graph>::Create(int n) {
	typedef std::pair<int, int> E;

	E wn_edges[2*(n-1)];
	int i;
	int c = 0;

	for (i = 0; i < n - 1; i++) {
		if(i == n-2){
			wn_edges[c++] = E(i, 0);
		}
		else{
			wn_edges[c++] = E(i, i + 1);
		}

		wn_edges[c++] = E(i, n - 1);
	}

	Graph wn(&wn_edges[0], &wn_edges[0] + sizeof(wn_edges) / sizeof(E), n);
	return wn;
}
