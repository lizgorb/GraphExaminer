/*
 * MaxClique.hpp
 *
 *  Created on: Jun. 18, 2019
 *      Author: liz
 */

#ifndef SRC_ALG_MAX_CLIQUE_HPP_
#define SRC_ALG_MAX_CLIQUE_HPP_

#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iostream>

class MaxClique {
public:
	MaxClique();

	template <typename Graph>
	static int NaiveMaxCliqueNumber(const Graph &g){
		return bron_kerbosch_clique_number(g);
	}

	template <typename Graph>
	static int MaxCliqueNumberWithSEOrder(const Graph &g, std::vector<int> order);

	template <typename Graph>
	static int LinearMaxCliqueNumberWithSEOrder(const Graph &g, std::vector<int> order);

};

template <typename Graph>
int MaxClique::MaxCliqueNumberWithSEOrder(const Graph &g, std::vector<int> order){

	std::cout << "MaxCliqueNumberWithSEOrder" << std::endl;

	 int nV = boost::num_vertices(g);
	 int clique_size[nV] = {0};
	 clique_size[nV-1] = 1;
	 typename Graph::adjacency_iterator vi, vi_end;

	 for (int j=0; j <= nV-2; j++){
		 int i = order[nV-2-j];
		 //get smallest larger neighbor

		 bool found = false;
		 int sl = 0;
		 int ll = 0;
		 int neighbors = 0;
		 for(int k=nV-2-j; k < nV; k++){
			 if(boost::edge(i, order[k], g).second){
				 if (!found){
					 found = true;
					 sl = k;
				 }

				 ll = k;

				 neighbors++;
			 }
		 }

		 // if the vertex is simplicial in G[Vi,..Vn]
		 if(j < 1 || neighbors > 2 || (neighbors == 2 && boost::edge(order[sl], order[ll], g).second)){
			 clique_size[nV - 2- j] = clique_size[sl] + 1;
		 }
		 else{
			 // 1 neighbor or 2 non adjacent neighbors
			 clique_size[nV - 2- j] = 2;
		 }
	 }

	 int max_clique = *std::max_element(clique_size, clique_size + nV);

	 return max_clique;
}

template <typename Graph>
int MaxClique::LinearMaxCliqueNumberWithSEOrder(const Graph &g, std::vector<int> order){

	std::cout << "LinearMaxCliqueNumberWithSEOrder" << std::endl;
	typename Graph::adjacency_iterator vi, vi_end;

	 int nV = boost::num_vertices(g);
	 int clique_size[nV] = {0};
	 clique_size[nV-1] = 1;

	 // array checked
	bool checked[nV] = {false};

	// to store wether a vertex is simplicial or not
	bool simplicial[nV] = {false};

	// array flags - neighbors
	bool flags[nV] = {false};

	// store smallest neighbor
	int smallest_neighbor[nV] = {nV};

	// store smallest neighbor
	int order_degrees[nV] = {0};

	// first - SE candidate, second - edge to test adjacency to
	typedef std::pair<int, int> SEC;
	std::vector<SEC> adjacency_check[nV];

	// array - vertex -> order
	int order_map[nV];
	for (int i=0; i<nV; i++){
		order_map[order[i]] = i;
	}

	// for i -> n-1, no need to check the last one. order O(n)
	for (int i=0; i< nV -1 ; i++){
			// k is the vertex
			int k = order[i];

			// mark as checked
			checked[k] = true;

			std::vector<int> neighbors;

			// index of smallest neighbor of k in the order
			int j = nV;

			//  iterate adjacent vertices -  o(deg(i))
			for (boost::tie(vi, vi_end) = boost::adjacent_vertices(k, g); vi != vi_end; vi++) {
				if(!checked[*vi]){

					// 	set flags - o(deg(i)) - using adjacency list
					flags[*vi] = true;

					//  store neighbors in [vi, ... vn] (as list, and count)
					neighbors.push_back(*vi);

					//  store nearest smallest neighbor
					if(order_map[*vi] < j){
						j = order_map[*vi];
					}
				}
			}

			smallest_neighbor[i] = j;
			order_degrees[i] = neighbors.size();

			// if the degree is larger than 2 the vertex in order[i] must be simplicial
			if(neighbors.size() > 2){
				simplicial[i] = true;
			}
			// if i is of degree 2, ask j to decide wethere it's simplicial
			else if(neighbors.size() == 2){

				// find the other neighbor O(1)
				int second_neighbor = neighbors[0];
				if (second_neighbor == order[j]){
					second_neighbor = neighbors[1];
				}

				adjacency_check[j].push_back(SEC(i, second_neighbor));
			}
			else{ // vertices of degree 1 are not simplicial
				simplicial[i] = true;
			}

			// check all simplicial extreme candidates
			for(SEC candidate :  adjacency_check[i]){

				// if the second vertex not adjacent to the current vertex than
				// the vertex in order[candidate.first] is not simplicial
				if(!flags[candidate.second]){
					simplicial[candidate.first] = false;
				}
				else{
					simplicial[candidate.first] = true;
				}

			}

			// clean up flags - done in O(n) should be done in O(deg(i))
			for(int i=0; i<nV; i++){
				flags[i] = false;
			}
		}

	// calc clique sizes
	for (int i=nV-1; i>=0 ; i--){
		if(simplicial[i]){
			clique_size[i] = order_degrees[i] + 1;
		}
		else
		{
			clique_size[i] = 2;
		}
	}

	 int max_clique = *std::max_element(clique_size, clique_size + nV);

	 return max_clique;
}

#endif /* SRC_ALG_MAX_CLIQUE_HPP_ */
