/*
 * common_test.cpp
 *
 *  Created on: Jun. 17, 2019
 *      Author: liz
 */
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../src/common_utils.hpp"


BOOST_AUTO_TEST_CASE(CommonUtils_AllSubsets)
{
	std::vector<std::vector<int>> subsets = CommonUtils::AllSubsets(2);
	sort(subsets.begin(), subsets.end());
	std::vector<std::vector<int>> expected = {{},{0},{0,1},{1}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::AllSubsets(3);
	sort(subsets.begin(), subsets.end());
	expected = {{},{0},{0,1},{0,1,2},{0,2},{1},{1,2},{2}};
	BOOST_CHECK(subsets == expected);
}


BOOST_AUTO_TEST_CASE(CommonUtils_SubsetsOfSize_With_Range)
{
	std::vector<std::vector<int>> subsets = CommonUtils::SubsetsOfSize(4, 4);
	sort(subsets.begin(), subsets.end());
	std::vector<std::vector<int>> expected = {{0,1,2,3}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::SubsetsOfSize(4, 3);
	sort(subsets.begin(), subsets.end());
	expected = {{0,1,2},{0,1,3},{0,2,3},{1,2,3}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::SubsetsOfSize(4, 2);
	sort(subsets.begin(), subsets.end());
	expected = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::SubsetsOfSize(4, 1);
	sort(subsets.begin(), subsets.end());
	expected = {{0},{1},{2},{3}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::SubsetsOfSize(4, 0);
	expected = {{}};
	BOOST_CHECK(subsets == expected);
}

BOOST_AUTO_TEST_CASE( CommonUtils_SubsetsOfSize_With_Set)
{
	std::vector<int> set =  { 5, 7, 8, 16};
	std::vector<std::vector<int>> subsets = CommonUtils::SubsetsOfSize(set, 2);
	sort(subsets.begin(), subsets.end());
	std::vector<std::vector<int>> expected = {{5,7},{5,8},{5,16},{7,8},{7,16},{8, 16}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::SubsetsOfSize(set, 3);
	sort(subsets.begin(), subsets.end());
	expected = {{5,7,8},{5,7,16},{5,8,16},{7,8,16}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::SubsetsOfSize(set, 4);
	sort(subsets.begin(), subsets.end());
	expected = {{5,7,8,16}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::SubsetsOfSize(set, 1);
	expected = {{5},{7},{8},{16}};
	BOOST_CHECK(subsets == expected);

	subsets = CommonUtils::SubsetsOfSize(set, 0);
	expected = {{}};
	BOOST_CHECK(subsets == expected);
}
