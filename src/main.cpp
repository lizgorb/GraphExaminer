/*
 * main.cpp
 *
 *  Created on: 2018 M02 15
 *      Author: Liz
 */
#include "graph_examiner.h"
#include <string>
#include <vector>
#include <execinfo.h>
#include <signal.h>
#include <ucontext.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main(int argc, char **argv){
	signal(SIGSEGV, handler);

	if(argc > 3){
		int max_v = std::stoi(argv[1]);
		int color_limit = std::stoi(argv[2]);
		std::vector<std::string> subgraphs(argv + 3, argv + argc);
		GraphExaminer ge;

		auto start = std::chrono::system_clock::now();
		std::time_t start_time = std::chrono::system_clock::to_time_t(start);
		std::cout << "Started at " << std::ctime(&start_time);

		ge.Examine(max_v, color_limit, subgraphs);

		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		std::cout << "Ended at " << std::ctime(&end_time);
	}

	return 0;
}


