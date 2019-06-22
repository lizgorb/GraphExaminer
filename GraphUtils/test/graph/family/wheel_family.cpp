#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/graph/isomorphism.hpp>
#include "../../../src/defs.hpp"
#include "../../../src/graph_utils.hpp"
#include "../../../src/graph/family/wheel_family.hpp"

BOOST_AUTO_TEST_CASE(WheelFamily_Create)
{
	Graph result = WheelFamily<Graph>::Create(3);
	Graph empty;
	BOOST_CHECK(boost::isomorphism(result, empty));

	Graph type;
	result = WheelFamily<Graph>::Create(4);
	string edge_list =  "0--1 0--2 0--3 1--2 2--3 3--1";
	Graph expected = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(boost::isomorphism(result, expected));

	result = WheelFamily<Graph>::Create(7);
	edge_list =  "0--1 0--2 0--3 0--4 0--5 0--6 1--2 2--3 3--4 4--5 5--6 6--1";
	expected = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(boost::isomorphism(result, expected));
}
