/*
 *  common.cpp
 *
 *  Created on: Jun 13, 2018
 *      Author: liz
 */

#ifndef COMMON_UTILS_HPP_
#define COMMON_UTILS_HPP_

#include <vector>

using namespace std;

class CommonUtils {
public:
	static vector<vector<int> > AllSubsets(int n) {
	vector<int> set;
	for (int i = 0; i < n; i++)
		set.push_back(i);

	vector<vector<int> > subset;
	vector<int> empty;
	subset.push_back(empty);

	for (int i = 0; i < set.size(); i++) {
		vector<vector<int> > subsetTemp = subset;

		for (int j = 0; j < subsetTemp.size(); j++)
			subsetTemp[j].push_back(set[i]);

		for (int j = 0; j < subsetTemp.size(); j++)
			subset.push_back(subsetTemp[j]);
	}
	return subset;
}

	static vector< vector<int>> SubsetsOfSize(vector<int> set,  int k)
	{
		vector< vector<int> > subset;

		vector<int> empty;
		subset.push_back( empty );

		for (int i = 0; i < set.size(); i++)
		{
			vector< vector<int> > subsetTemp = subset;

			for (int j = 0; j < subsetTemp.size(); j++)
				subsetTemp[j].push_back( set[i] );

			for (int j = 0; j < subsetTemp.size(); j++)
				subset.push_back( subsetTemp[j] );
		}

		vector< vector<int> > subset_filtered;
		for (int i = 0; i < subset.size(); i++){
			if (subset[i].size()  == k){
				subset_filtered.push_back(subset[i]);
			}
		}

		return subset_filtered;
	}

	static vector< vector<int>> SubsetsOfSize(int n,  int k)
	{
		vector<int> set;
		for ( int i = 0; i < n; i++ )
			  set.push_back(i);

		return SubsetsOfSize(set, k);
	}
};


#endif /* COMMON_UTILS_HPP_ */
