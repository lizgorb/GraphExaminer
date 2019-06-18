#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GraphUtils
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "../src/defs.hpp"
#include "../src/graph_utils.hpp"

BOOST_AUTO_TEST_CASE( GraphUtils_PrintGraph )
{
	string expected =  "0--1 0--2 0--3 1--3 3--4 3--5 2--6 5--6 0--7 1--8 6--8 5--9 6--9 7--9 8--9";
	E test_edges[] = { E(0,1),E(0,2),E(0,3),E(1,3), E(3,4), E(3,5), E(2,6),
				 E(5,6), E(0,7),E(1,8), E(6,8), E(5,9),  E(6,9), E(7,9),  E(8,9) };

	Graph test (&test_edges[0], &test_edges[0] + sizeof(test_edges) / sizeof(E), 10);

	std::ostringstream os;
	GraphUtils::PrintGraph(test, os);
	string result = os.str();
	boost::trim_right(result);

	BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE( GraphUtils_FromString )
{
	string edge_list =  "0--1 0--2 0--3 1--3 3--4 3--5 2--6 5--6 0--7 1--8 6--8 5--9 6--9 7--9 8--9";
	Graph type;
	Graph test = GraphUtils::FromString(edge_list, type);

	std::ostringstream os;
	GraphUtils::PrintGraph(test, os);
	string result = os.str();
	boost::trim_right(result);

	BOOST_CHECK(result == edge_list);
}

BOOST_AUTO_TEST_CASE( GraphUtils_IsSimplicial )
{
	string edge_list =  "0--1 0--3 1--3 3--4 3--5 2--6 2--8 5--6 0--7 1--8 6--8 5--9 6--9 7--9 8--9";
	Graph type;
	Graph test = GraphUtils::FromString(edge_list, type);

	BOOST_CHECK(GraphUtils::IsSimplicial(test, 2) == true);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 4) == true);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 0) == false);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 1) == false);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 3) == false);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 5) == false);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 6) == false);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 7) == false);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 8) == false);
	BOOST_CHECK(GraphUtils::IsSimplicial(test, 9) == false);
}


