#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/graph/isomorphism.hpp>
#include "../../src/defs.hpp"
#include "../../src/graph_utils.hpp"
#include "../../src/graph/common_graphs.hpp"


BOOST_AUTO_TEST_CASE(CommonGraphs_Contains)
{
	BOOST_CHECK(CommonGraphs::Contains("bowtie"));
	BOOST_CHECK(CommonGraphs::Contains("diamond"));
	BOOST_CHECK(CommonGraphs::Contains("kite"));
	BOOST_CHECK(CommonGraphs::Contains("bicycle"));
	BOOST_CHECK(CommonGraphs::Contains("unknown") == false);
}

BOOST_AUTO_TEST_CASE(CommonGraphsCreator_Create)
{
	CommonGraphsCreator<Graph> creator;
	Graph type;
	string edge_list =  "0--1 0--2 1--2 0--3 0--4 3--4";
	Graph expected_graph = GraphUtils::FromString(edge_list, type);
	Graph result_graph = creator.Create("bowtie");
	BOOST_CHECK(boost::isomorphism(expected_graph, result_graph));

	edge_list =  "0--1 0--2 1--2 1--3 2--3";
	expected_graph = GraphUtils::FromString(edge_list, type);
	result_graph = creator.Create("diamond");
	BOOST_CHECK(boost::isomorphism(expected_graph, result_graph));

	edge_list =  "0--1 0--2 1--2 1--3 2--3 0--4";
	expected_graph = GraphUtils::FromString(edge_list, type);
	result_graph = creator.Create("kite");
	BOOST_CHECK(boost::isomorphism(expected_graph, result_graph));

	edge_list =  "0--1 0--2 1--2 0--3 3--4 3--5 4--5";
	expected_graph = GraphUtils::FromString(edge_list, type);
	result_graph = creator.Create("bicycle");
	BOOST_CHECK(boost::isomorphism(expected_graph, result_graph));

	Graph empty_graph;
	result_graph = creator.Create("unknown");
	BOOST_CHECK(boost::isomorphism(empty_graph, result_graph));
}

