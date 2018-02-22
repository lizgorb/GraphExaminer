/*
 * clique_inducer.h
 *
 *  Created on: Jan 28, 2018
 *      Author: liz
 */

#ifndef UTILS_INDUCERS_CLIQUE_INDUCER_H_
#define UTILS_INDUCERS_CLIQUE_INDUCER_H_

#include "basic_inducer.h"
#include <iostream>
#include <algorithm>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>

class InducedCliqueVisitor
{
private:

	 vector<int>& clique_sizes_;

public:
	 InducedCliqueVisitor(vector<int>& clique_sizes): clique_sizes_(clique_sizes) {}

    int max_clique_size(){return *max_element(std::begin(clique_sizes_), std::end(clique_sizes_)); }

    template <typename Clique, typename Graph>
    void clique(const Clique& c, const Graph& g)
    {
    	int size=0;
        // Iterate over the clique and print each vertex within it.
        typename Clique::const_iterator i, end = c.end();
        for(i = c.begin(); i != end; ++i) {
           // std::cout << *i << " ";
            size++;
        }
       // std::cout << endl;
        clique_sizes_.push_back(size);

    }
};

template <typename Graph>
class CliqueInducer : public BasicInducer<Graph>  {
public:
	CliqueInducer(int n);
	bool IsInduced(const Graph& g, const Graph& subgraph);
private:
	int clique_size;
};

#endif /* UTILS_INDUCERS_CLIQUE_INDUCER_H_ */
