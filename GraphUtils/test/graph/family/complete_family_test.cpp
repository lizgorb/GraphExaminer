#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../../../src/defs.hpp"
#include "../../../src/graph/family/complete_family.hpp"


BOOST_AUTO_TEST_CASE(CompleteFamily_Create)
{
	Graph result = CompleteFamily<Graph>::Create(1);
	BOOST_CHECK(boost::num_vertices(result) == 1);
	BOOST_CHECK(boost::num_edges(result) == 0);

	result = CompleteFamily<Graph>::Create(2);
	BOOST_CHECK(boost::num_vertices(result) == 2);
	BOOST_CHECK(boost::num_edges(result) == 1);

	result = CompleteFamily<Graph>::Create(13);
	BOOST_CHECK(boost::num_vertices(result) == 13);
	BOOST_CHECK(boost::num_edges(result) == 13*12/2);
}
