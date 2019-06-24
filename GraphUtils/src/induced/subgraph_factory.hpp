/*
 * SubGraphFactory.h
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#ifndef GRAPH_SUBGRAPH_FACTORY_H_
#define GRAPH_SUBGRAPH_FACTORY_H_

#include <string>

#include "subgraph.hpp"
#include "inducer/basic_inducer.hpp"
#include "inducer/dominating_vertex_inducer.cpp"
#include "inducer/clique_inducer.cpp"
#include "inducer/path_inducer.cpp"
#include "inducer/diamond_inducer.cpp"
#include "../graph/family/complete_family.cpp"
#include "../graph/family/wheel_family.cpp"
#include "../graph/family/path_family.cpp"

#include <iostream>
#include <regex>
#include "common_graphs.hpp"

using namespace std;

template <typename Graph>
class SubgraphFactory {
public:
	static Subgraph<Graph> Create( string name );
};

template <typename Graph>
Subgraph<Graph> SubgraphFactory<Graph>::Create(string name){
	std::regex reg_complete("(k)(\\d+)");
	std::regex reg_wheel("(w)(\\d+)");
	std::regex reg_path("(p)(\\d+)");
	std::string::size_type sz;
	// todo Use map<regex, creator>, register all creator on load
	// (possibly include dynamic loading)

	Graph g;
	BasicInducer<Graph> *inducer = new BasicInducer<Graph>();
	if (CommonGraphs::Contains(name)){
		g = CommonGraphsCreator<Graph>::Create(name);
		//todo Create inducer factory add strategy to graph families/common graphs
		if (name == "bowtie"){
			inducer = new DominatingVertexInducer<Graph>(4);
		}
		else if(name == "diamond"){
			inducer = new DiamondInducer<Graph>();
		}
	}
	else if(regex_match (name, reg_complete))
	{
		int n = stoi(name.substr(1), &sz);
		g = CompleteFamily<Graph>::Create(n);
		inducer = new CliqueInducer<Graph>;
	}
	else if(regex_match (name, reg_wheel))
	{
		int n = stoi(name.substr(1), &sz);
		g = WheelFamily<Graph>::Create(n);
		inducer = new DominatingVertexInducer<Graph>(n-1);
	}
	else if(regex_match (name, reg_path))
		{
			int n = stoi(name.substr(1), &sz);
			g = PathFamily<Graph>::Create(n);
			inducer = new PathInducer<Graph>(n);
		}
	else{
		std::cout << name << " not found " << endl;
	}

	Subgraph<Graph> sg(g, inducer, name);
	return sg;
}


#endif /* GRAPH_SUBGRAPH_FACTORY_H_ */
