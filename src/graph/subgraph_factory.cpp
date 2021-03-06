/*
 * SubGraphFactory.cpp
 *
 *  Created on: 2018 M01 21
 *      Author: Liz
 */

#include "subgraph_factory.h"
#include "common_graphs.h"

#include "family/complete_family.cpp"
#include "family/wheel_family.cpp"
#include "family/path_family.cpp"
#include "../utils/inducers/basic_inducer.h"
#include "../utils/inducers/dominating_vertex_inducer.cpp"
#include "../utils/inducers/clique_inducer.cpp"
#include "../utils/inducers/path_inducer.cpp"
#include "../utils/inducers/diamond_inducer.cpp"

#include <iostream>
#include <regex>

template <typename Graph>
Subgraph<Graph> SubgraphFactory<Graph>::Create(string name){
	regex reg_complete("(k)(\\d+)");
	regex reg_wheel("(w)(\\d+)");
	regex reg_path("(p)(\\d+)");
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
		inducer = new CliqueInducer<Graph>(n);
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
