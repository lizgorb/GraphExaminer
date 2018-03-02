/*
 * diamond_inducer.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: liz
 */

#include "diamond_inducer.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <iostream>

using namespace boost;
using namespace std;

template <typename Graph>
bool DiamondInducer<Graph>::IsInduced(const Graph& g, const Graph& subgraph){
	int last_vertex_i = num_vertices(g) - 1;
	// induce the neighborhood of the last vertex
	vector<int> neighbors = this->GetNeighborhood(g, last_vertex_i);
	Graph ngbGraph = this->InduceSubset(g, neighbors);

	// find all connected componants in the neighborhood
    std::vector<int> component(num_vertices(ngbGraph));
    int numComponants = connected_components(ngbGraph, &component[0]);

    for (int i = 0; i != component.size(); ++i)
        std::cout << "Vertex " << i <<" is in component " << component[i] << endl;
    std::cout << endl;

	return false;
}
