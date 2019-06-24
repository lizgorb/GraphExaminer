/*
 * diamond_inducer.h
 *
 *  Created on: Mar 2, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_DIAMOND_INDUCER_H_
#define UTILS_INDUCERS_DIAMOND_INDUCER_H_

#include "../inducer/basic_inducer.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <iostream>

using namespace boost;

template <typename Graph>
class DiamondInducer : public BasicInducer<Graph>  {
public:
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	bool IsCentralVertex(const Graph& g, int i);
};

template <typename Graph>
bool DiamondInducer<Graph>::IsInduced(const Graph& g, const Graph& subgraph){
	bool induced = false;

	int n_v = num_vertices(g);
	if(n_v < 4  || num_edges(g) < 5 ){
		return induced;
	}

	int last_vertex_i = n_v - 1;

	// consider central node
	if(IsCentralVertex(g, last_vertex_i)){
		induced = true;
	}

	// consider side node
	if(!induced){
		// the neighborhood consists of disjoint cliques
		vector<int> neighbors = this->GetNeighborhood(g,last_vertex_i);
		sort(neighbors.begin(), neighbors.end());

		// for all pairs
		for (int i=0; i < neighbors.size() && !induced; i++ ){
			for (int j=i+1; j < neighbors.size() && !induced; j++ ){

				int v = neighbors[i];
				int y = neighbors[j];

				// if they are part of the same component
				if(boost::edge(v, y, g).second){
					vector<int> neighbors_v = this->GetNeighborhood(g,v);
					vector<int> neighbors_y = this->GetNeighborhood(g,y);

					// find their common neighbors
					 sort(neighbors_v.begin(), neighbors_v.end());
					 sort(neighbors_y.begin(), neighbors_y.end());
					 vector<int> intersection;
					 set_intersection(neighbors_v.begin(),neighbors_v.end(),
							 neighbors_y.begin(),neighbors_y.end(),
							 back_inserter(intersection));

					 // if they have more than one
					 if(intersection.size() > 1){
						 // that is not a neighbor of the current vertex- it's a diamond
						 sort(intersection.begin(), intersection.end());

						 vector<int> cliqueIntersection;
						 set_intersection(intersection.begin(),intersection.end(),
								 neighbors.begin(),neighbors.end(),
								 back_inserter(cliqueIntersection));
						 if(cliqueIntersection.size() <  intersection.size() - 1 ){
							 induced = true;
						 }
					 }
				}
			}
		}
	}

	return induced;
}

template <typename Graph>
bool DiamondInducer<Graph>::IsCentralVertex(const Graph& g, int i){
	bool central = false;
	// induce the neighborhood of the last vertex
	vector<int> neighbors = this->GetNeighborhood(g, i);

	// step 1 - considering central node
	if (neighbors.size() >= 3){
		Graph ngbGraph = this->InduceSubset(g, neighbors);

		// find all connected components in the neighborhood
		std::vector<int> component(num_vertices(ngbGraph));
		int numComponants = connected_components(ngbGraph, &component[0]);
		vector<int> componants[numComponants];
		for (int i = 0; i != component.size(); ++i){
			componants[component[i]].push_back(neighbors[i]);
		}

		// check that all connected components are cliques
		for(auto c : componants){
			int componentSize = c.size();

			// no point checking single vertex graphs and edges
			if(componentSize > 2){
				Graph temp = this->InduceSubset(g, c);
				int cliqueEdgeSize = componentSize*(componentSize -1)/2;
				if(boost::num_edges(temp) < cliqueEdgeSize){
					central = true;
					break;
				}
			}
		}
	}

	return central;
}

#endif /* UTILS_INDUCERS_DIAMOND_INDUCER_H_ */
