/*
 * simplicial_extreme.h
 *
 *  Created on: Jun 15, 2018
 *      Author: liz
 */

#ifndef SRC_SEARCH_SIMPLICIAL_EXTREME_HPP_
#define SRC_SEARCH_SIMPLICIAL_EXTREME_HPP_

#include "simplicial_extreme.hpp"

#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <set>
#include <iostream>

#include "../graph_utils.hpp"


class SimplicialExtreme {
public:

	template <typename Graph>
	static std::vector<int> GenerateOrder(const Graph &g);

	template <typename Graph>
	static std::vector<int> LinearGenerateOrder(const Graph &g);

	template <typename Graph>
	static bool VerifyOrder(const Graph &g, std::vector<int> order);


	template <typename Graph>
	static bool LinearVerifyOrder(const Graph &g, std::vector<int> order);

	template <typename Graph>
	static bool LinearVerifyOrder2(const Graph &g, std::vector<int> order);

	template <typename Graph>
	static bool LinearVerifyAllOrders(const Graph &g, std::vector<std::vector<int>> order);
};

template <typename Graph>
std::vector<int> SimplicialExtreme::GenerateOrder(const Graph &g)
{
	//cout << "generating SE order" << endl;

	// create the order in O(nm)
	std::vector<int> order;
	int nV =  boost::num_vertices(g);
	Graph temp(g);
	bool removed[nV] = {false};

	while(order.size() < nV -1){
		int found = -1;

		for (int i=0; i < boost::num_vertices(temp) && (found == -1); i++){
			int degree = boost::degree(i, temp);
			if (!removed[i]){
				if (boost::degree(i, temp) <= 2){

					order.push_back(i);
					found = i;
				}
				else if(GraphUtils::IsSimplicial<Graph>(temp, i)){

					order.push_back(i);
					found = i;
				}
			}
		}

		// if not SE vertex exists - return the incomplete order
		if (found == -1)
		{
			return order;
		}

		boost::clear_vertex(found, temp);
		removed[found] = true;
	}

	// add the last vertex
	for(int i=0; i < nV; i++){
		if(!removed[i])
		{
			order.push_back(i);
		}
	}

	return order;
}

template <typename Graph>
std::vector<int> SimplicialExtreme::LinearGenerateOrder(const Graph &g)
{
	// cout << "Linear generating SE order" << endl;

	typename Graph::adjacency_iterator vi, vi_end;

	std::vector<int> order;
	int nV = boost::num_vertices(g);
	int degrees[nV];
	bool labeled[nV];

	for (int i = 0; i < nV; i++) {
		degrees[i] = boost::degree(i, g);
	}

	for (int i = 0; i < nV; i++) {
		// find min degree vertex
		int min = nV;
		int min_index = nV;
		for (int j=0; j < nV; j++){
			if(!labeled[j] && degrees[j] < min){
				min = degrees[j];
				min_index = j;
			}
		}
		std::cout << min;

		// add to order
		labeled[min_index] = true;
		order.push_back(min_index);


		// update adjacent
		for (boost::tie(vi, vi_end) = adjacent_vertices(min_index, g); vi != vi_end; ++vi)
		{
			if(!labeled[*vi])
			{
				//std::cout << "updating neighbor: " << *vi << endl;
				degrees[*vi] -= 1;
			}
		}
	}
	std::cout << std::endl;

	return order;
}


template <typename Graph>
bool SimplicialExtreme::VerifyOrder(const Graph &g, std::vector<int> order){
	typename Graph::adjacency_iterator vi, vi_end;
	bool valid = true;
	int nV = boost::num_vertices(g);
	Graph temp;
	boost::copy_graph(g, temp);


	for(int i=0; i < order.size() && valid; i++){
		int k=order[i];
		int deg_k = boost::degree(k, temp);

		if(GraphUtils::IsSimplicial<Graph>(temp, k)){
			boost::clear_vertex(k, temp);

		}
		else if(deg_k == 2){

			//check if there is an edge between the two neighbors
			std::vector<int> neighbors;

			boost::tie(vi, vi_end) = boost::adjacent_vertices(k, temp);
			for (; vi != vi_end; vi++) {
				neighbors.push_back(*vi);
			}

			if(boost::edge(neighbors[0], neighbors[1], temp).second){
				// if there is - i is not a simplicial extreme
				valid  = false;
			}
			else{
				boost::clear_vertex(k, temp);
			}

		}
		else if (deg_k > 2){
			valid = false;
			boost::tie(vi, vi_end) = boost::adjacent_vertices(k, temp);
						for (; vi != vi_end; vi++) {
							//cout << *vi << " ";
						}
		}
	}


	return valid;
}


template <typename Graph>
bool SimplicialExtreme::LinearVerifyAllOrders(const Graph &g, std::vector<std::vector<int>> orders){
	 bool valid = true;
	 for(int i =0; i<orders.size() && valid; i++)
	 {
		 valid = SimplicialExtreme::LinearVerifyOrder(g, orders[i]);
	 }
	return valid;
}


template <typename Graph>
bool SimplicialExtreme::LinearVerifyOrder(const Graph &g, std::vector<int> order){
	typename Graph::adjacency_iterator vi, vi_end;

	bool valid = true;
	int nV = boost::num_vertices(g);

	 // array checked
	bool checked[nV] = {false};

	// array flags - neighbors
	bool flags[nV] = {false};

	// array - vertex -> order
	int order_map[nV];
	for (int i=0; i<nV; i++){
		order_map[order[i]] = i;
	}

	// array<vector<int>> bba
	std::set<int> bba[nV];
	bool bhap[nV];

	// for i -> n-1, no need to check the last one. order O(n)
	for (int i=0; i< nV -1 && valid; i++){
		// k is the vertex
		int k = order[i];

		// mark as checked
		checked[k] = true;

		std::vector<int> neighbors;

		// index of smallest neighbor of g in the order
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

		// 	check bba - O(deg(x))
		for (int adj_req : bba[i]) {

			// if there is no edge and it's not to itself
			if (!flags[adj_req] && adj_req != k )
			{
				valid = false;
				break;
			}

			if(!valid){
				break;
			}
		}


		// if still valid
		if(valid){
			// 	if deg in [vi,..,vn] is 2 - no need to pass
			//  else append to bba(sn)
			if(neighbors.size() > 2){
				//std::set<int> s_neighbors(neighbors.begin(), neighbors.end());
				bba[j].insert(neighbors.begin(), neighbors.end());
			}

			if(neighbors.size() == 3){
				bhap[j] = true;
			}

			// if the current vertex has 2 neighbors (x, y) and they were both passed
			// in it's bba by the same vertex - then there is another vertex that requires x and y to adjacent
			// in this case pass them forward
			else if (neighbors.size() == 2 && bhap[i]){
				bba[j].insert(neighbors.begin(), neighbors.end());
			}

		}

		// clean up flags - done in O(n) should be done in O(deg(i))
		for(int i=0; i<nV; i++){
			flags[i] = false;
		}
	}

	return valid;
}

#endif /* SRC_SEARCH_SIMPLICIAL_EXTREME_HPP_ */
