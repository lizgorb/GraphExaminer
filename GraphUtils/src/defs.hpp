#ifndef TEST_COMMON_HPP_
#define TEST_COMMON_HPP_

#include <boost/graph/adjacency_list.hpp>


typedef typename boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef typename std::pair<int, int> E;


#endif /* TEST_COMMON_HPP_ */
