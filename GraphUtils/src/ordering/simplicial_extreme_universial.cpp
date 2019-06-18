/*
 * simplicial_extreme_universial.cpp
 *
 *  Created on: Nov 17, 2018
 *      Author: liz
 */

#include "simplicial_extreme_universial.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>

#include "../graph_utils.hpp"
#include "../search/mcs.cpp"

/* DIDNT WORK
template <typename Graph>
vector<int> SimplicialExtremeUniversial::GenerateOrder(const Graph &g){
	std::vector<int> order;
	int nV = boost::num_vertices(g);
	typename Graph::adjacency_iterator vi, vi_end;

	std::string lex[nV];

	bool numbered[nV] = {false};

	int degrees[nV];
	for(int i = nV; i >= 1; i--){
		degrees[i] = boost::degree(i, g);
	}

	std::string max;
	int max_index = 0;
	int max_deg = nV;

	for(int i = nV; i >= 1; i--){
		// find unlabeled with max adjacency - this is O(n) but can be improved
		max = "";
		max_index = 0;
		max_deg = nV;
		for (int i=0; i < nV; i++)
		{
			if(!numbered[i] and LBFS::LexCompare(lex[i], max) >= 0)
			{
			   //break ties using lowest degree
				if(boost::degree(i,g) < max_deg){
					max = lex[i];
					max_index = i;
					max_deg = boost::degree(i, g);
				}
			}
		}

		// add vertex with largest label to order and mark it as "numbered"
		order.push_back(max_index);
		numbered[max_index] = true;

		// update "unnumbered" adjacent vertices
		for (boost::tie(vi, vi_end) = adjacent_vertices(max_index, g); vi != vi_end; ++vi)
		{
			if(!numbered[*vi])
			{
				if (lex[*vi] != "")
				{
					lex[*vi] += ".";
				}
				lex[*vi] += std::to_string(i);

			}
		}

	}

	std::reverse(order.begin(),order.end());
	return order;
}
*/

template <typename Graph>
vector<int> SimplicialExtremeUniversial::GenerateOrder(const Graph &g){
	std::vector<int> order;
	int nV = boost::num_vertices(g);
	typename Graph::adjacency_iterator vi, vi_end;
	Graph temp;
	boost::copy_graph(g, temp);

	std::vector<int> lbfsorder;
	lbfsorder = MCS::GenerateOrder(g);
	int lbfs_curr = 0;

	bool numbered[nV] = {false};

	int degrees[nV];
	for(int i = nV; i >= 0; i--){
		degrees[i] = boost::degree(i, g);
	}


	for(int i = nV; i >= 1; i--){
		int next = -1;

		// see if we have a universal vertex
		for (int i=0; i < nV; i++)
		{
			if(!numbered[i])
			{
				if(degrees[i] == nV - order.size() -1 or degrees[i] <= 2){
					next = i;
					break;
				}

			}
		}

		// if not grab the next unnumbered from lbfs
		if(next == -1){
			next = lbfsorder[lbfs_curr];
			while(numbered[next]){
				lbfs_curr += 1;
				next = lbfsorder[lbfs_curr];
			}
		}


		// add vertex with largest label to order and mark it as "numbered"
		order.push_back(next);
		numbered[next] = true;

		// update "unnumbered" adjacent vertices
		for (boost::tie(vi, vi_end) = adjacent_vertices(next, g); vi != vi_end; ++vi)
		{
			if(!numbered[*vi])
			{
				degrees[*vi] -= 1;
			}
		}

	}
	return order;
}


template <typename Graph>
bool SimplicialExtremeUniversial::VerifyOrder(const Graph &g, vector<int> order){
	typename Graph::adjacency_iterator vi, vi_end;
	bool valid = true;
	int nV = boost::num_vertices(g);
	Graph temp;
	boost::copy_graph(g, temp);


	for(int i=0; i < order.size() && valid; i++){
		int k=order[i];
		int deg_k = boost::degree(k, temp);

		// if the vertex is simplicial or universial - clear it
		if(GraphUtils::IsSimplicial<Graph>(temp, k) or deg_k == (order.size() - i - 1)){
			boost::clear_vertex(k, temp);

		}
		else if(deg_k == 2){

			//check if there is an edge between the two neighbors
			vector<int> neighbors;

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
		// if it is not universal
		else if (deg_k > 2 and deg_k < (order.size() - i - 1)){
			valid = false;
			boost::tie(vi, vi_end) = boost::adjacent_vertices(k, temp);
		}
	}


	return valid;
}


