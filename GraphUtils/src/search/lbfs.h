/*
 * lbfs.h
 *
 *  Created on: Jun 21, 2018
 *      Author: liz
 */

#ifndef SRC_SEARCH_LBFS_H_
#define SRC_SEARCH_LBFS_H_
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

class LBFS {
public:
	template <typename Graph>
	static std::vector<int> GenerateOrder(const Graph &g);

	template <typename Graph>
	static std::vector<int> GenerateOrder(const Graph &g, int start_index);

	template <typename Graph>
	static std::vector<std::vector<int>> GenerateAllOrders(const Graph &g);

	template <typename Graph>
    static std::vector<std::vector<int>> GenerateAllOrders(const Graph &g, bool break_ties_randomly);

	template <typename Graph>
	static std::vector<std::vector<int>> GenerateAllSimplicialExtremeOrders(const Graph &g);

	template <typename Graph>
	static std::vector<std::vector<int>> BuildOrders(const Graph &g, std::vector<bool> numbered_in, std::vector<std::string> lex_in, std::vector<int> order_in);

	static int LexCompare(std::string first, std::string second)
	{
		// return:
		// 1 if first > second
		// -1 if first < second
		// 0 if first == second
		int result = 0;
		if(first != second){
			if(first == ""){
				result = -1;
			}
			else if(second == ""){
				result = 1;
			}

			std::vector<std::string> strs_first;
			std::vector<std::string> strs_second;
			boost::split(strs_first, first, boost::is_any_of("."));
			boost::split(strs_second, second, boost::is_any_of("."));
			for(int i=0;
					i< strs_first.size() && i < strs_second.size() && result==0;
					i++){
				 int a = std::stoi (strs_first[i]);
				 int b = std::stoi (strs_second[i]);

				 if(a > b){
					 result = 1;
				 }
				 else if(a < b){
					 result = -1;
				 }
			}

			if(result ==0){
				if(strs_first.size() > strs_second.size())
				{
					result = 1;
				}
				else{
					result = -1;
				}
			}
		}


		return result;
	}

};

#endif /* SRC_SEARCH_LBFS_H_ */
