/*
 * HoleDetector.cpp
 *
 *  Created on: 2017 M12 15
 *      Author: Liz
 */

#include "cycle_inducer.h"

using namespace boost;

template <typename Graph>
vector<Graph> CycleInducer<Graph>::GetAllCycles(const Graph& g){
	vector<vector<int>> cycles;

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
	 cycle_collector<ostream> vis(cycles);
	 // Use the Tiernan algorithm to visit all cycles
	 tiernan_all_cycles(udg, vis);

	 vector<Graph> induced_cycles;
	 for(vector<int> cycle: cycles){
		 induced_cycles.push_back(this->InduceSubset(g, cycle));
	 }

	 return induced_cycles;
}


template <typename Graph>
int CycleInducer<Graph>::MaxCycle(const Graph& g){
	vector<vector<int>> cycles;

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
	 cycle_collector<ostream> vis(cycles);
	 // Use the Tiernan algorithm to visit all cycles
	 tiernan_all_cycles(udg, vis);

	 return vis.max_length_cycle();
}
