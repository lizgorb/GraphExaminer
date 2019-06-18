#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../../src/defs.hpp"
#include "../../src/graph_utils.hpp"
#include "../../src/alg/max_clique.hpp"

BOOST_AUTO_TEST_CASE(MaxClique_Naive)
{
	Graph type;
	string edge_list =  "0--1 1--2 2--3";
	Graph test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(MaxClique::NaiveMaxCliqueNumber(test) == 2);

	edge_list =  "0--1 0--2 1--2";
	test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(MaxClique::NaiveMaxCliqueNumber(test) == 3);
}
