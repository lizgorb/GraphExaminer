#include <iostream>
#include <time.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

#include "graph/subgraph_factory.cpp"
#include "graph_generator.cpp"
#include "induced_check.cpp"
#include "utils/colorer.cpp"
#include "utils/hole_detector.cpp"
using namespace boost;

template<typename Graph>
void print_graph(const Graph& g) {
	for (typename Graph::edge_iterator ei = edges(g).first;
			ei != edges(g).second; ei++) {
		std::cout << "(" << source(*ei, g) << "-" << target(*ei, g) << ")";
	}
	std::cout << endl;
}

int main(int argc, char **argv) {
	clock_t t;
	t = clock();

	typedef adjacency_list<vecS, vecS, undirectedS> Graph;
	typedef std::pair<int, int> E;
	typedef graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typedef graph_traits<Graph>::vertices_size_type vertices_size_type;
	typedef property_map<Graph, vertex_index_t>::const_type vertex_index_map;

	E edges[] = { E(0, 1) };
	Graph vector_graph(&edges[0], &edges[0] + sizeof(edges) / sizeof(E), 2);

	GraphGenerator<Graph> gen;
	InducedCheck<Graph> checker;
	HoleDetector<Graph> hd;
	Colorer<Graph> colorer;
	map<int, int> color_map;
	Subgraph<Graph> k4 = SubgraphFactory<Graph>::Create("k4");
	checker.AddInducedGraph(k4);
	Subgraph<Graph> bowtie = SubgraphFactory<Graph>::Create("bowtie");
	checker.AddInducedGraph(bowtie);
	Subgraph<Graph> w6 = SubgraphFactory<Graph>::Create("w6");
	checker.AddInducedGraph(w6);

	/*E test_edges[] = { E(0,1),E(0,2),E(0,3),E(1,4),E(0,5),E(1,5),E(2,5),E(3,5),E(2,6),E(4,6),E(3,7),E(4,7),E(6,7) };
	 Graph test (&test_edges[0], &test_edges[0] + sizeof(test_edges) / sizeof(E), 6);
	 cout << "3 colorable? " << std::boolalpha << colorer.ThreeColor(test,color_map) << endl;
	 cout << "has induced? " << std::boolalpha << checker.CheckInduced(test) << endl;*/

	//generate graphs
	Graph curr;
	vector<Graph> graphs;
	queue<Graph> q;
	q.push(vector_graph);

	int count = 0;
	while (!q.empty()) {
		curr = q.front();
		q.pop();
		if (boost::num_vertices(curr) + 1 <= 10) {
			//if less than n vertices
			// std::cout << "Adding vertex to: ";
			graphs = gen.addVertex(curr);
			for (auto &g : graphs) {
				if (!checker.CheckInduced(g) && !hd.CheckEvenHole(g)) {
					q.push(g);

					//  huristic - try 3 coloring
					vector<vertices_size_type> color_vec(num_vertices(g));
					iterator_property_map<vertices_size_type*, vertex_index_map> color(
							&color_vec.front(), get(vertex_index, g));
					vertices_size_type num_colors = sequential_vertex_coloring(
							g, color);
					if (num_colors > 3) {
						color_map.clear();
						bool colorable = colorer.ThreeColor(g, color_map);

						// really check 3 coloring
						if (!colorable) {
							count++;
							cout << "3 colorable? " << std::boolalpha
									<< colorable << " : ";
							print_graph(g);
						}
					}
				}
			}
		}
	}

	std::cout << "Found " << count << " non 3 colorable graphs" << endl;
	gen.benchmark();
	checker.benchmark();
	hd.benchmark();
	colorer.benchmark();

	std::cout << "Took " << ((float) (clock() - t)) / CLOCKS_PER_SEC
			<< " seconds" << endl;

	return 0;
}
