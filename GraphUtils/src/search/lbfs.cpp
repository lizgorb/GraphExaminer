/*
 * lbfs.cpp
 *
 *  Created on: Jun 21, 2018
 *      Author: liz
 */

#include "lbfs.h"
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string.hpp>


template <typename Graph>
std::vector<int> LBFS::GenerateOrder(const Graph &g)
{
	return LBFS::GenerateOrder(g, 0);
}

template <typename Graph>
std::vector<std::vector<int>> LBFS::GenerateAllOrders(const Graph &g)
{
	return LBFS::GenerateAllOrders(g, true);
}

template <typename Graph>
std::vector<std::vector<int>> LBFS::GenerateAllOrders(const Graph &g,  bool break_ties_randomly)
{
	typename Graph::adjacency_iterator vi, vi_end;
	std::vector<std::vector<int>> orders;

	int nV = boost::num_vertices(g);
	std::vector<bool> numbered(nV);
	std::vector<std::string> lex(nV);
	std::vector<int> order;

	for(int i=0; i<boost::num_vertices(g); i++){
		if(break_ties_randomly){
			orders.push_back(LBFS::GenerateOrder(g, i));
		}
		else{
			numbered = std::vector<bool> (nV, false);
			lex = std::vector<std::string> (nV, "");
			order.clear();

			// init start vertex
			lex[i] = "-1";
			numbered[i] = true;
			order.push_back(i);
			// update adjacent vertices
			for (boost::tie(vi, vi_end) = adjacent_vertices(i, g); vi != vi_end; ++vi)
			{
				if (lex[*vi] != "")
				{
					lex[*vi] += ".";
				}
				lex[*vi] += std::to_string(nV);
			}

			std::vector<std::vector<int>> curr_orders = BuildOrders(g, numbered, lex, order);
			orders.insert( orders.end(), curr_orders.begin(), curr_orders.end() );
		}
	}
	return orders;
}

template <typename Graph>
std::vector<int> LBFS::GenerateOrder(const Graph &g, int start_index)
{
	std::vector<int> order;
	int nV = boost::num_vertices(g);
	typename Graph::adjacency_iterator vi, vi_end;

	std::string lex[nV];

	bool numbered[nV] = {false};

	std::string max;
	int max_index = 0;

	lex[start_index] = std::to_string(nV);

	for(int i = nV; i >= 1; i--){
		// find unlabeled with max adjacency - this is O(n) but can be improved
		max = "";
		max_index = 0;
		for (int i=0; i < nV; i++)
		{
			if(!numbered[i] && LexCompare(lex[i], max) == 1)
			{
				max = lex[i];
				max_index = i;
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


template <typename Graph>
std::vector<std::vector<int>> LBFS::BuildOrders(const Graph &g, std::vector<bool> numbered_in, std::vector<std::string> lex_in, std::vector<int> order_in)
{
	int nV = boost::num_vertices(g);
	typename Graph::adjacency_iterator vi, vi_end;
	std::vector<std::vector<int>> orders;

	if(order_in.size() == nV){
		std::reverse(order_in.begin(),order_in.end());
		orders.push_back(order_in);
	}
	else{
		// copy the vectors
		std::vector<bool> numbered = numbered_in;
		std::vector<std::string> lex = lex_in;
		std::vector<int> order = order_in;

		// find the max lex vertices
		std::vector<int> max_indexes;

		for (int i=0; i < nV; i++)
		{
			if(!numbered[i]){
				// add the first one
				if(max_indexes.empty()){
					max_indexes.push_back(i);
				}
				// collect all max indexes
				else{
					int comp = LexCompare(lex[i], lex[max_indexes[0]]);
					if(comp >= 0)
					{
						if(comp == 1){
							max_indexes.clear();
						}
						max_indexes.push_back(i);
					}
				}
			}
		}

		// choose all max options as next
		for(int index: max_indexes)
		{
			order.push_back(index);
			numbered[index] = true;
			std::vector<std::string> lex_temp = lex;
			// update "unnumbered" adjacent vertices
			for (boost::tie(vi, vi_end) = adjacent_vertices(index, g); vi != vi_end; ++vi)
			{
				if(!numbered[*vi])
				{
					if (lex[*vi] != "")
					{
						lex[*vi] += ".";
					}
					lex[*vi] += std::to_string(nV-order.size());
				}
			}
			std::vector<std::vector<int>> rem_orders = BuildOrders(g,numbered,lex,order);
			orders.insert( orders.end(), rem_orders.begin(), rem_orders.end() );
			order.pop_back();
			numbered[index] = false;
			lex = lex_temp;
		}
	}

	return orders;
}

template <typename Graph>
std::vector<std::vector<int>> LBFS::GenerateAllSimplicialExtremeOrders(const Graph &g)
{
	std::vector<std::vector<int>> orders;
	std::vector<int> order;
	int nV = boost::num_vertices(g);
	typename Graph::adjacency_iterator vi, vi_end;



	int numbered_count =0;
	std::vector<int> cleanup;
	std::vector<int> cleanup_copy;

	bool numbered_cleanup[nV] = {false};

	int degrees[nV] = {0};

	for (int i = 0; i < nV; i++) {
		degrees[i] = boost::degree(i, g);
	}

	// cleanup - remove initial vertices with degrees 1 or 2
	bool has_low_degrees = true;
	while(has_low_degrees){
		has_low_degrees = false;
		for(int i=0; i<nV; i++){
			if(degrees[i] <= 2 && !numbered_cleanup[i]){
				cleanup.push_back(i);
				numbered_cleanup[i] = true;
				numbered_count++;
				has_low_degrees = true;
				for (boost::tie(vi, vi_end) = adjacent_vertices(i, g); vi != vi_end; ++vi)
				{
					if(!numbered_cleanup[*vi])
					{
						degrees[*vi] -= 1;
					}
				}
			}
		}
	}

	std::string max;
	int max_index = 0;
	int start_index;

	// start at all unnumbered
	for(int j=0; j<nV; j++){
		bool numbered[nV] = {false};
		std::copy(numbered_cleanup, numbered_cleanup + nV, numbered);

		if(!numbered[j]){
			std::string lex[nV];

			order.clear();
			// copy cleanup order
			cleanup_copy.clear();
			cleanup_copy.insert( cleanup_copy.end(), cleanup.begin(), cleanup.end() );

			start_index = j;
			lex[start_index] = std::to_string(nV-numbered_count);

			for(int i = nV-numbered_count; i >= 1; i--){
				max = "";
				max_index = 0;
				for (int i=0; i < nV; i++)
				{
					if(!numbered[i] && LexCompare(lex[i], max) == 1)
					{
						max = lex[i];
						max_index = i;
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
			cleanup_copy.insert( cleanup_copy.end(), order.begin(), order.end() );
			orders.push_back(cleanup_copy);
		}
	}

	return orders;
}


