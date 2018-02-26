/*
 * main.cpp
 *
 *  Created on: 2018 M02 15
 *      Author: Liz
 */
#include "graph_examiner.h"
#include <string>


int main(int argc, char **argv){
	int max_v = 10;
	if(argc>=2)
	 {
		max_v = std::stoi(argv[1]);
	 }

	GraphExaminer ge;
	ge.Examine(max_v);
	return 0;
}

