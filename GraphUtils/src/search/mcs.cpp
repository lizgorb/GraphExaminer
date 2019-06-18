/*
 * mcs.cpp
 *
 *  Created on: Jun 13, 2018
 *      Author: liz
 */

#include "mcs.h"
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <algorithm>    // std::reverse


template <typename Graph>
vector<vector<int>> MCS::GenerateAllOrders(const Graph &g)
{
	vector<vector<int>> orders;
	for(int i =0; i < boost::num_vertices(g); i++){
		vector<int> order;
		order = MCS::GenerateOrder(g, i);
		orders.push_back(order);
	}

	return orders;
}


template <typename Graph>
vector<int> MCS::GenerateOrder(const Graph &g)
{
	return MCS::GenerateOrder(g, 0);
}

template <typename Graph>
vector<int> MCS::GenerateOrder(const Graph &g, int start_index)
{
	vector<int> order;
	typename Graph::adjacency_iterator vi, vi_end;
	int nV = boost::num_vertices(g);

	bool labeled[nV] = {false};
	int labeled_count = 0;
	int adjacency_counts[nV] = {0};

	int max = 0;
	int max_index = start_index;


	while (labeled_count < nV){
		order.push_back(max_index);
		//std::cout << "labeling: " << max_index << endl;
		labeled[max_index] = true;
		labeled_count += 1;
		// update adjacent vertices
		for (boost::tie(vi, vi_end) = adjacent_vertices(max_index, g); vi != vi_end; ++vi)
		{
			if(!labeled[*vi])
			{
				//std::cout << "updating neighbor: " << *vi << endl;
				adjacency_counts[*vi] += 1;
			}
		}

		// find unlabeled with max adjacency - this is O(n) but can be improved
		max = 0;
		max_index = 0;
		for (int i=0; i < nV; i++)
		{
			if(!labeled[i] && adjacency_counts[i] >= max)
			{
				max = adjacency_counts[i];
				max_index = i;
			}
		}

		//std::cout << "next vertex: " << max_index << endl;
	}

	std::reverse(order.begin(),order.end());
	return order;
}


template <typename Graph>
vector<vector<int>> MCS::GenerateAllSimplicialExtremeOrders(const Graph &g)
{
	vector<vector<int>> orders;
	vector<int> order;

	vector<int> cleanup;
	vector<int> cleanup_copy;
	typename Graph::adjacency_iterator vi, vi_end;
	int nV = boost::num_vertices(g);

	bool labeled_cleanup[nV] = {false};
	int labeled_count = 0;

	int degrees[nV] = {0};

	for (int i = 0; i < nV; i++) {
		degrees[i] = boost::degree(i, g);
	}

	int max = 0;
	int max_index = 0;

	// cleanup - remove initial vertices with degrees 1 or 2
	bool has_low_degrees = true;
	while(has_low_degrees){
		has_low_degrees = false;
		for(int i=0; i<nV; i++){
			if(degrees[i] <= 2 && !labeled_cleanup[i]){
				cleanup.push_back(i);
				labeled_cleanup[i] = true;
				labeled_count++;
				has_low_degrees = true;
				for (boost::tie(vi, vi_end) = adjacent_vertices(i, g); vi != vi_end; ++vi)
				{
					if(!labeled_cleanup[*vi])
					{
						degrees[*vi] -= 1;
					}
				}
			}
		}
	}

	// start from all unlabeled
	for(int j=0; j<nV; j++){

		bool labeled[nV] = {false};
		std::copy(labeled_cleanup, labeled_cleanup + nV, labeled);

		if(!labeled[j]){
			int adjacency_counts[nV] = {0};

			max_index = j;

			order.clear();
			// copy cleanup order
			cleanup_copy.clear();
			cleanup_copy.insert( cleanup_copy.end(), cleanup.begin(), cleanup.end() );

			labeled_count = cleanup.size();
			while (labeled_count < nV){
				order.push_back(max_index);
				//std::cout << "labeling: " << max_index << endl;
				labeled[max_index] = true;
				labeled_count += 1;
				// update adjacent vertices
				for (boost::tie(vi, vi_end) = adjacent_vertices(max_index, g); vi != vi_end; ++vi)
				{
					if(!labeled[*vi])
					{
						//std::cout << "updating neighbor: " << *vi << endl;
						adjacency_counts[*vi] += 1;
					}
				}

				// find unlabeled with max adjacency - this is O(n) but can be improved
				max = 0;
				max_index = 0;
				for (int i=0; i < nV; i++)
				{
					if(!labeled[i] && adjacency_counts[i] >= max)
					{
						max = adjacency_counts[i];
						max_index = i;
					}
				}
			}

			std::reverse(order.begin(),order.end());
			cleanup_copy.insert( cleanup_copy.end(), order.begin(), order.end() );

			orders.push_back(cleanup_copy);
		}
	}

	return orders;
}

