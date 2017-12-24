/*
 * HoleDetector.cpp
 *
 *  Created on: 2017 M12 15
 *      Author: Liz
 */

#include "HoleDetector.h"

using namespace boost;

template <typename Graph>
bool HoleDetector<Graph>::CheckEvenHole (const Graph& g){
	_CheckEvenHoleCalls++;
	clock_t t;
	t = clock();

	 // convert to undirected graph
	 UDGraph udg;

	 int nV = num_vertices(g);

	 int i;
	 graph_traits<UDGraph>::vertex_descriptor vetices[nV];
	 for(i = 0; i < nV; i++){
		 vetices[i] = udg.add_vertex();
	 }
	 for (typename Graph::edge_iterator ei = edges(g).first; ei != edges(g).second; ei++) {
		 udg.add_edge(vetices[source(*ei, g)], vetices[target(*ei,g)]);
	 }

	 vector<vector<int>> even_holes;
	 hole_checker<ostream> vis(even_holes);
	 vis.has_even_hole();
	 // Use the Tiernan algorithm to visit all cycles
	 tiernan_all_cycles(udg, vis);

	 _CheckEvenHoleSeconds += ((float)(clock() - t))/CLOCKS_PER_SEC;
	 return vis.has_even_hole();
}
