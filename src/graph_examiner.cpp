#include "graph_examiner.h"
#include <time.h>
#include <boost/graph/sequential_vertex_coloring.hpp>

void GraphExaminer::Examine(int max_v, int color_limit, vector<string> subgraphs) {
	clock_t t;
	t = clock();


	colorer.setColorLimit(color_limit);

	// initialize queue with K2
	E k2_edges[] = { E(0, 1) };
	Graph k2(&k2_edges[0], &k2_edges[0] + sizeof(k2_edges) / sizeof(E), 2);
	graphs_queue_.push(k2);


	for(auto sg_name : subgraphs){
		Subgraph<Graph> sg = SubgraphFactory<Graph>::Create(sg_name);
		checker.AddInducedGraph(sg);
	}


	/*E crit1_edges[] = { E(0,1),E(0,2),E(0,3),E(1,4),E(0,5),E(1,5),E(2,5),E(3,5),
				 E(2,6),E(4,6),E(3,7),E(4,7),E(6,7) };
		 Graph crit1 (&crit1_edges[0], &crit1_edges[0] + sizeof(crit1_edges) / sizeof(E), 8);
		 BasicInducer<Graph> *inducer = new BasicInducer<Graph>();
		 Subgraph<Graph> sg(crit1, inducer, "crit1");
	checker.AddInducedGraph(sg); */



	  // tests
	 /*
	 max_v = 0;
     map<int, int> color_map;
	 E test_edges[] = { E(0,1),E(0,3),E(1,2),E(1,3), E(2,3)};
	  Graph test (&test_edges[0], &test_edges[0] + sizeof(test_edges) / sizeof(E), 4);
	  colorer.ThreeColor(test, color_map);
	  PrintColoring(color_map);

	Subgraph<Graph> p_test = SubgraphFactory<Graph>::Create("diamond");
	 if(p_test.IsInduced(test)){  cout << "diamond is induced" << endl; }
	 else{ cout << "diamond is not induced" << endl; }
	 */

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
			CheckGraphs(graphs, color_limit);
		}
	}

	string f_free;
	for (auto const& s : subgraphs) { f_free += s + ","; }
	std::cout << "Examined (" << f_free << ")-free up to "  << max_v << " vertices" << endl;
	std::cout << "Found " << graph_count_ << " non "<< color_limit << " colorable graphs" << endl;
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

void GraphExaminer::PrintDegrees(const Graph& g) {
	std::cout << "Degrees: ";
	for (int i = 0; i < num_vertices(g); i++) {
		std::cout << boost::degree(i, g) << ",";
	}
	std::cout << endl;
}

void GraphExaminer::PrintColoring(map<int, int> color_map) {
	for(auto it = color_map.cbegin(); it != color_map.cend(); ++it)
	{
		std::cout << it->first << ":" << it->second << ", ";
	}
	std::cout<<endl;
}

void GraphExaminer::CheckGraphs(vector<Graph> graphs, int color_limit){

	#pragma omp parallel for
	for(int x=0; x < graphs.size(); x++)	{
	//for (Graph &g : graphs) {
		Graph g = graphs[x];
		if (CheckGraph(g, color_limit))
		{
			#pragma omp critical
			{graphs_queue_.push(g);}
		}
	}
};

bool GraphExaminer::CheckGraph(const Graph& g, int color_limit){
	bool valid = false;
	map<int, int> color_map;

	if (!checker.CheckInduced(g) && !hd.CheckEvenHole(g)) {
		valid = true;
		//  huristic - try k coloring
		vector<vertices_size_type> color_vec(num_vertices(g));
		iterator_property_map<vertices_size_type*, vertex_index_map> color(
				&color_vec.front(), get(vertex_index, g));
		vertices_size_type num_colors = sequential_vertex_coloring(
				g, color);
		if (num_colors > color_limit) {
			color_map.clear();
			bool colorable = colorer.ThreeColor(g, color_map);
			//PrintDegrees(g);

			// really check k coloring
			if (!colorable) {
				#pragma omp critical
				{
					graph_count_++;
					cout << "Is " << color_limit<< " colorable? " << std::boolalpha << colorable << " : ";
					PrintGraph(g);
					BasicInducer<Graph> *inducer = new BasicInducer<Graph>();
					Subgraph<Graph> sg(g, inducer, "crit" + to_string(graph_count_));
					checker.AddInducedGraph(sg);


					// no need to keep growing this graph
					valid = false;
				}
			}

//			if(num_vertices(g) == 11){
//				PrintGraph(g);
//				PrintColoring(color_map);
//			}
		}
	}

	return valid;
};
