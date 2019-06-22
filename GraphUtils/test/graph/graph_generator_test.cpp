#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/graph/isomorphism.hpp>
#include <vector>
#include <iostream>
#include "../../src/defs.hpp"
#include "../../src/graph_utils.hpp"
#include "../../src/graph/graph_generator.hpp"

BOOST_AUTO_TEST_CASE(GraphGenerator_AddVertex)
{
	GraphGenerator<Graph> gen;
	Graph type;
	string edge_list =  "0--1";
	Graph graph = GraphUtils::FromString(edge_list, type);
	vector<Graph> extended = gen.AddVertex(graph);

	BOOST_CHECK(extended.size() == 2);
	// P_3, triangle
	Graph p3 = GraphUtils::FromString("0--1 1--2", type);
	Graph triangle = GraphUtils::FromString("0--1 1--2 0--2", type);
	vector<Graph> expected_graphs = {p3, triangle};
	int found = 0;
	for (Graph curr : extended){
		for(Graph expected : expected_graphs)
		{
			if(boost::isomorphism(expected, curr))
			{
				found += 1;
			}
		}
	}
	BOOST_CHECK(found == 2);

	edge_list =  "0--1 1--2";
	graph = GraphUtils::FromString(edge_list, type);
	extended = gen.AddVertex(graph);
	BOOST_CHECK(extended.size() == 5);

	// P_4, C_4, claw, paw, diamond
	Graph p4 = GraphUtils::FromString("0--1 1--2 2--3", type);
	Graph c4 = GraphUtils::FromString("0--1 1--2 2--3 3--0", type);
	Graph claw = GraphUtils::FromString("0--1 0--2 0--3", type);
	Graph paw = GraphUtils::FromString("0--1 0--2 0--3 1--2", type);
	Graph diamond = GraphUtils::FromString("0--1 0--2 1--2 1--3 2--3", type);
	expected_graphs = {p4, c4, claw, paw, diamond};

	found = 0;
	for (Graph curr : extended){
		for(Graph expected : expected_graphs)
		{
			if(boost::isomorphism(expected, curr))
			{
				found += 1;
			}
		}
	}
	BOOST_CHECK(found == 5);
}
