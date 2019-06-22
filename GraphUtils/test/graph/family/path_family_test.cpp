#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/graph/isomorphism.hpp>
#include "../../../src/defs.hpp"
#include "../../../src/graph_utils.hpp"
#include "../../../src/graph/family/path_family.hpp"

BOOST_AUTO_TEST_CASE(PathFamily_Create)
{
	Graph result = PathFamily<Graph>::Create(1);
	BOOST_CHECK(boost::num_vertices(result) == 1);
	BOOST_CHECK(boost::num_edges(result) == 0);

	result = PathFamily<Graph>::Create(2);
	BOOST_CHECK(boost::num_vertices(result) == 2);
	BOOST_CHECK(boost::num_edges(result) == 1);

	result = PathFamily<Graph>::Create(5);
	Graph type;
	string edge_list =  "0--1 1--2 2--3 3--4";
	Graph expected = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(boost::isomorphism(result, expected));
}

