/*
 * CommonGraphs.cpp
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#include "common_graphs.h"

E CommonGraphs::diamond_edges[] = { E (0, 1), E (0, 2), E (1, 2), E(1,3), E (2,3) };
CommonGraph CommonGraphs::diamond = {4, 5, CommonGraphs::diamond_edges};
E CommonGraphs::bowtie_edges[] = { E (0, 1), E (0, 2), E (1, 2), E (2,3), E (2, 4), E (3, 4) };
CommonGraph CommonGraphs::bowtie = {5, 6, CommonGraphs::bowtie_edges};
E CommonGraphs::kite_edges[] = { E (0, 1), E (0, 2), E (1, 2), E(1,3), E (2,3), E(3, 4) };
CommonGraph CommonGraphs::kite = {5, 6, CommonGraphs::kite_edges};
E CommonGraphs::bicycle_edges[] = { E(0,1),E(0,2),E(1,2),E(2,3),E(3,4),E(3,5),E(4,5) };
CommonGraph CommonGraphs::bicycle = {5, 7, CommonGraphs::bicycle_edges};

map<string, CommonGraph> CommonGraphs::common_graphs_ = {
    { "bowtie",   CommonGraphs::bowtie },
	{ "diamond", CommonGraphs::diamond },
	{ "kite",   CommonGraphs::kite },
	{ "bicycle",   CommonGraphs::bicycle }
};

bool CommonGraphs::Contains ( string name ){
	bool c = false;
	if (common_graphs_.count(name)){
		c = true;
	}
	return c;
}

CommonGraph CommonGraphs::Get ( string name ){
	return common_graphs_[name];
}
