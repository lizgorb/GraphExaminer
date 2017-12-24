/*
 * CompleteClass.cpp
 *
 *  Created on: 2017 M11 6
 *      Author: Liz
 */

#include "CompleteClass.h"
#include <boost/graph/adjacency_list.hpp>

template <typename Graph>
CompleteClass<Graph>::CompleteClass() {
	// TODO Auto-generated constructor stub

}

template <typename Graph>
CompleteClass<Graph>::~CompleteClass() {
	// TODO Auto-generated destructor stub
}

template <typename Graph>
Graph get(int n){
	typedef std::pair <int, int> E;

	 E kn_edges[n*(n-1)];
	 int i,j;
	 int c = 0;

	 for(i=0; i <n; i++){
		 for(j=0; j <n; j++){
				 kn_edges[c] = E(i,j);
				 c++;
			 }
	 }
	 Graph kn (&kn_edges[0], &kn_edges[0] + sizeof(kn_edges) / sizeof(E), n);
	 return kn;
}
