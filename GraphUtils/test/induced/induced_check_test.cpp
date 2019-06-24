#include <boost/test/unit_test.hpp>
#include "../../src/defs.hpp"
#include "../../src/induced/induced_check.hpp"

BOOST_AUTO_TEST_CASE( InducedCheck_CheckInduced )
{
	// 0--1 0--2 0--3 1--3 3--4 3--5 2--6 5--6 0--7 1--8 6--8 5--9 6--9 7--9 8--9
	E test_edges[] = { E(0,1),E(0,2),E(0,3),E(1,3), E(3,4), E(3,5), E(2,6),
			 E(5,6), E(0,7),E(1,8), E(6,8), E(5,9),  E(6,9), E(7,9),  E(8,9) };

	Graph test (&test_edges[0], &test_edges[0] + sizeof(test_edges) / sizeof(E), 10);

	InducedCheck<Graph> checker;

	// 0--1 0--2 0--3 1--4 0--5 1--5 2--5 3--5 2--6 4--6 3--7 4--7 6--7
	E crit1_edges[] = { E(0,1),E(0,2),E(0,3),E(1,4),E(0,5),E(1,5),E(2,5),E(3,5),
				 E(2,6),E(4,6),E(3,7),E(4,7),E(6,7) };

	Graph crit1 (&crit1_edges[0], &crit1_edges[0] + sizeof(crit1_edges) / sizeof(E), 8);
	BasicInducer<Graph> *inducer = new BasicInducer<Graph>();
	Subgraph<Graph> sg(crit1, inducer, "crit1");
	checker.AddInducedGraph(sg);

    BOOST_CHECK( checker.CheckInduced(test) == false );

    // 0--1 0--2 0--3 0--4 1--4 2--4 1--5 5--6 3--6
	E crit2_edges[] = { E(0,1), E(0,2), E(0,3), E(0,4),
						E(1,4), E(2,4), E(1,5), E(5,6),
						E(3,6)};
	Graph crit2 (&crit2_edges[0], &crit2_edges[0] + sizeof(crit2_edges) / sizeof(E), 7);
	Subgraph<Graph> sg2(crit2, inducer, "crit2");
	checker.AddInducedGraph(sg2);

	BOOST_CHECK( checker.CheckInduced(test) == true );
}



