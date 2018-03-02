#include "graph_examiner.h"
#include <time.h>
#include <boost/graph/sequential_vertex_coloring.hpp>

void GraphExaminer::Examine(int max_v, vector<string> subgraphs) {
	clock_t t;
	t = clock();

	// initialize queue with K2
	E k2_edges[] = { E(0, 1) };
	Graph k2(&k2_edges[0], &k2_edges[0] + sizeof(k2_edges) / sizeof(E), 2);
	graphs_queue_.push(k2);


	for(auto sg_name : subgraphs){
		Subgraph<Graph> sg = SubgraphFactory<Graph>::Create(sg_name);
		checker.AddInducedGraph(sg);
	}

	 /*
	  // tests
	  E test_edges[] = { E(0,1),E(0,2),E(0,3),E(1,4),E(0,5),E(1,5),E(2,6),E(0,7),
				 E(2,7),E(3,7),E(5,7),E(1,8),E(4,8), E(5,8), E(6,8) };
	 Graph test (&test_edges[0], &test_edges[0] + sizeof(test_edges) / sizeof(E), 9);
	 Subgraph<Graph> p_test = SubgraphFactory<Graph>::Create("p5");
	 if(p_test.IsInduced(test)){  cout << "path is induced" << endl; }
	 else{ cout << "path is not induced" << endl; }

	 /*vector<vertices_size_type> color_vec(num_vertices(test));
	iterator_property_map<vertices_size_type*, vertex_index_map> color(
			&color_vec.front(), get(vertex_index, test));
	vertices_size_type num_colors = sequential_vertex_coloring(test, color);
	 cout << "test - sequential 3 colorable?" << num_colors << endl;
	 cout << "test - 3 colorable? " << std::boolalpha << colorer.ThreeColor(test,color_map) << endl;
	 cout << "has induced? " << std::boolalpha << checker.CheckInduced(test) << endl;*/

	//generate graphs
	Graph curr;
	vector<Graph> graphs;

	while (!graphs_queue_.empty()) {
		curr = graphs_queue_.front();
		graphs_queue_.pop();
		if (boost::num_vertices(curr) + 1 <= max_v) {
			graphs = gen.AddVertex(curr);
			CheckGraphs(graphs);
		}
	}

	string f_free;
	for (auto const& s : subgraphs) { f_free += s + ","; }
	std::cout << "Examined (" << f_free << ")-free up to "  << max_v << " vertices" << endl;
	std::cout << "Found " << graph_count_ << " non 3 colorable graphs" << endl;
	gen.benchmark();
	checker.benchmark();
	hd.benchmark();
	colorer.benchmark();

	std::cout << "Took " << ((float) (clock() - t)) / CLOCKS_PER_SEC
			<< " seconds" << endl;

	/**/

}

void GraphExaminer::PrintGraph(const Graph& g) {
	for (typename Graph::edge_iterator ei = edges(g).first;
		ei != edges(g).second; ei++) {
		std::cout <<  source(*ei, g) << "--" << target(*ei, g) << " ";
	}
	std::cout << endl;
}

void GraphExaminer::CheckGraphs(vector<Graph> graphs){

	#pragma omp parallel for
	for(int x=0; x < graphs.size(); x++)	{
	//for (Graph &g : graphs) {
		Graph g = graphs[x];
		if (CheckGraph(g))
		{
			#pragma omp critical
			{graphs_queue_.push(g);}
		}
	}
};

bool GraphExaminer::CheckGraph(const Graph& g){
	bool valid = false;
	if (!checker.CheckInduced(g) && !hd.CheckEvenHole(g)) {
		valid = true;

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
				graph_count_++;
				cout << "Is 3 colorable? " << std::boolalpha << colorable << " : ";
				PrintGraph(g);

				// no need to keep growing this graph
				valid = false;
			}
		}
	}

	return valid;
};
