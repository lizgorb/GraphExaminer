#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../../src/defs.hpp"
#include "../../src/graph_utils.hpp"
#include "../../src/alg/coloring.hpp"

BOOST_AUTO_TEST_CASE(Coloring_Greedy)
{
	Graph type;
	string edge_list =  "0--1 1--2";
	Graph test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(Coloring<Graph>::Greedy(test) == 2);

	edge_list =  "0--1 1--2 2--3";
	test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(Coloring<Graph>::Greedy(test) == 2);

	edge_list =  "0--2 2--3 3--1";
	test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(Coloring<Graph>::Greedy(test) == 3);
}

BOOST_AUTO_TEST_CASE(Coloring_Greedy_With_Order)
{
	Graph type;
	Coloring<Graph> colorer;
	vector<int> order = {0,2,3,1};
	string edge_list = "0--2 2--3 3--1";
	Graph test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(Coloring<Graph>::Greedy(test,order) == 2);
}

BOOST_AUTO_TEST_CASE(Coloring_Greedy_With_SL)
{
	Graph type;
	string edge_list = "0--2 2--3 3--1";
	Graph test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(Coloring<Graph>::GreedySmallestLast(test) == 2);
}

BOOST_AUTO_TEST_CASE(Colorable_Three_Color)
{
	Graph type;
	Coloring<Graph> colorer;
	map<int, int> color_map;
	string edge_list = "0--1 0--2 1--2";
	Graph test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(colorer.Colorable(test, color_map) == true);

	edge_list = "0--1 0--2 1--2 1--3 2--3";
	test = GraphUtils::FromString(edge_list, type);
	color_map.clear();
	BOOST_CHECK(colorer.Colorable(test, color_map) == true);

	// petersen graph
	edge_list = "0--1 1--2 2--3 3--4 4--0 0--5 5--6 1--6 6--7 2--7 7--8 3--8 8--9 4--9 9--5";
	test = GraphUtils::FromString(edge_list, type);
	color_map.clear();
	BOOST_CHECK(colorer.Colorable(test, color_map) == true);

	edge_list = "0--1 0--2 0--3 1--2 1--3 2--3";
	test = GraphUtils::FromString(edge_list, type);
	color_map.clear();
	BOOST_CHECK(colorer.Colorable(test, color_map) == false);
}

BOOST_AUTO_TEST_CASE(Colorable_Four_Color)
{
	Graph type;
	Coloring<Graph> colorer(4);
	map<int, int> color_map;
	string edge_list =  "0--1 0--2 1--2 0--3 1--3 2--3 2--4";
	Graph test = GraphUtils::FromString(edge_list, type);
	BOOST_CHECK(colorer.Colorable(test, color_map) == true);

	edge_list = "0--1 0--2 0--3 0--4 1--2 1--3 1--4 2--3 2--4 3--4";
	test = GraphUtils::FromString(edge_list, type);
	color_map.clear();
	BOOST_CHECK(colorer.Colorable(test, color_map) == false);
}
