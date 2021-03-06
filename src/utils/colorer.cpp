/*
 * Colorer.cpp
 *
 *  Created on: 2017 M12 17
 *      Author: Liz
 */

#include "colorer.h"

template<typename Graph>
bool Colorer<Graph>::ThreeColor(const Graph& g, map<int, int> &color_map) {
#pragma omp critical
	{
		_ThreeColorCalls++;
	}
	clock_t t;
	t = clock();
	map<int, int> _index_mapping;

	initColorMap(num_vertices(g), color_map);
	initIndexMap(num_vertices(g), _index_mapping);

	map<int, int> temp_color_map;
	bool valid_coloring = ThreeColor(g, temp_color_map, "-", _index_mapping,
			color_map);
#pragma omp critical
	{
		_ThreeColorSeconds += ((float) (clock() - t)) / CLOCKS_PER_SEC;
	}
	return valid_coloring;
}

template<typename Graph>
bool Colorer<Graph>::ThreeColor(const Graph& g, map<int, int> color_map,
		string token, map<int, int> &_index_mapping,
		map<int, int> &_final_color_map) {

	int nV = num_vertices(g);
	IndexMap indices = get(vertex_index, g);

	if (debug) {
		std::cout << token << ": " << "Checking " << _ColorLimit <<"-colorability of:";
		for (typename Graph::edge_iterator ei = edges(g).first;
				ei != edges(g).second; ei++) {
			std::cout << "(" << source(*ei, g) << "-" << target(*ei, g) << ")";
		}
		std::cout << endl;

		std::cout << token << ": ";
		for (const auto& c_pair : color_map) {
			std::cout << c_pair.first << ":" << c_pair.second << ", ";
		}
		std::cout << endl;
	}

	bool valid_coloring = false;
	// if the graph is a clique or all colors were assigned already
	if ((nV * (nV - 1) / 2) == num_edges(g)) {
		valid_coloring = valid_coloring || (nV <= _ColorLimit);

		// assign last colors

	} else {

		// find 2 non adjacent vertices
		bool found = false;
		int x, y;
		std::pair<adjacency_iterator, adjacency_iterator> neighbors;

		for (int i = 0; i < nV & !found; i++) {
			for (int j = i + 1; j < nV & !found; j++) {
				if (!boost::edge(get(indices, i), get(indices, j), g).second) {
					found = true;
					x = i;
					y = j;
				}
			}
		}

		if (debug) {
			std::cout << token << ": " << "Found non-adjacent " << x << " and "
					<< y << endl;
		}

		Graph temp;
		vector<int> x_colors = possibleColors(g, x, color_map);
		vector<int> y_colors = possibleColors(g, y, color_map);

		if (debug) {
			std::cout << " possible colors for " << x << ": ";
			for (auto i : x_colors)
				std::cout << i << ' ';
			std::cout << endl;
			std::cout << " possible colors for " << y << ": ";
			for (auto i : y_colors)
				std::cout << i << ' ';
			std::cout << endl;
		}

		// if can't color one of the vertices (or both) - not valid
		if (!(x_colors.size() > 0 && y_colors.size() > 0)) {
			return false;
		}

		vector<int> common;
		sort(x_colors.begin(), x_colors.end());
		sort(y_colors.begin(), y_colors.end());
		set_intersection(x_colors.begin(), x_colors.end(), y_colors.begin(),
				y_colors.end(), back_inserter(common));



		valid_coloring = ColorWithSameColor(g, color_map, token, x, y, x_colors,
				y_colors, _index_mapping, _final_color_map);


		//try to color them with different colors
		if (!valid_coloring) {
			valid_coloring = ColorWithDifferentColor(g, color_map, token, x, y,
					x_colors, y_colors, _index_mapping, _final_color_map);
		}
	}

	return valid_coloring;
}

template<typename Graph>
vector<int> Colorer<Graph>::possibleColors(const Graph& g, int v,
		map<int, int> color_map) {
	vector<int> pos;
	IndexMap indices = get(vertex_index, g);

	if (color_map.find(v) == color_map.end()) {
		vector<int> all = _AllColors; //{ 1, 2, 3 };
		std::pair<adjacency_iterator, adjacency_iterator> neighbors;
		neighbors = adjacent_vertices(v, g);

		// check adjacent colors
		for (; neighbors.first != neighbors.second; ++neighbors.first) {
			int n = get(indices, *neighbors.first);
			if (color_map.find(n) != color_map.end()) {
				// remove from all
				all.erase(std::remove(all.begin(), all.end(), color_map[n]),
						all.end());
			}
		}

		pos = all;
	} else {
		// if there is already a color assigned - we cannot change it
		pos.push_back(color_map[v]);
	}

	return pos;
}

template<typename Graph>
bool Colorer<Graph>::ColorWithSameColor(const Graph& g, map<int, int> color_map,
		string token, int x, int y, vector<int> x_colors, vector<int> y_colors,
		map<int, int> &_index_mapping, map<int, int> &_final_color_map) {
	bool valid_coloring = false;
	vector<int> common;
	set_intersection(x_colors.begin(), x_colors.end(), y_colors.begin(),
			y_colors.end(), back_inserter(common));
	Graph temp;
	int nV = num_vertices(g);

	// try to color them with the same color
	if (common.size() > 0) {
		int color = common.front();
		copy_graph(g, temp);
		color_map.insert(std::pair<int, int>(x, color));

		if (debug) {
			std::cout << token << ": " << "Colored " << x << " and " << y
					<< " with " << color << endl;
		}

		// copy all edges of y to x
		for (int i = 0; i < nV; i++) {
			if (boost::edge(y, i, temp).second) {
				// std::cout << "Removing edge " << y << " and " << i  << endl;
				boost::remove_edge(y, i, temp);

				if (!boost::edge(x, i, temp).second) {
					// std::cout << "Adding edge " << x << " and " << i  << endl;
					boost::add_edge(x, i, temp);
				}
			}
		}

		remove_vertex(y, temp);

		if (debug) {
			std::cout << token << ": " << "removed vertex " << y
					<< " new graph is: ";
			for (typename Graph::edge_iterator ei = edges(temp).first;
					ei != edges(temp).second; ei++) {
				std::cout << "(" << source(*ei, temp) << "-"
						<< target(*ei, temp) << ")";
			}
			std::cout << endl;
		}

		// storing coloring map before "removing" vertex - in case this won't work
		map<int, int> temp_color_map;
		temp_color_map.insert(color_map.begin(), color_map.end());

		// updating color map to remove vertex
		for (int i = y + 1; i < nV; i++) {
			if (color_map.find(i) != color_map.end()) {
				if (color_map.find(i - 1) != color_map.end())
					color_map[i - 1] = color_map[i];
				else
					color_map.insert(std::pair<int, int>(i - 1, color_map[i]));
			} else if (color_map.find(i - 1) != color_map.end()) {
				color_map.erase(i - 1);
			}
		}

		if (color_map.find(nV - 1) != color_map.end()) {
			color_map.erase(nV - 1);
		}

		// update index_map
		map<int, int> temp_index_map;
		temp_index_map.insert(_index_mapping.begin(), _index_mapping.end());
		for (int i = y; i < nV - 1; i++) {
			_index_mapping[i] = _index_mapping[i + 1];
		}

		valid_coloring = ThreeColor(temp, color_map,
				token + "-S(" + std::to_string(x) + "," + std::to_string(y)
						+ ")", _index_mapping, _final_color_map);

		// restore index_map
		_index_mapping = temp_index_map;


		if (valid_coloring) {
			_final_color_map[_index_mapping[x]] = color;
			_final_color_map[_index_mapping[y]] = color;
		}
		if (debug) {
			std::cout << token << ": " << "Valid coloring? " << std::boolalpha
					<< valid_coloring << endl;
			// print index_map
					for (const auto& c_pair : _index_mapping) {
						std::cout << c_pair.first << ":" << c_pair.second << ", ";
					}
					std::cout << endl;
		}

		// restore color map
		color_map = temp_color_map;
	}

	return valid_coloring;
}

template<typename Graph>
bool Colorer<Graph>::ColorWithDifferentColor(const Graph& g,
		map<int, int> color_map, string token, int x, int y,
		vector<int> x_colors, vector<int> y_colors,
		map<int, int> &_index_mapping, map<int, int> &_final_color_map) {
	bool valid_coloring;
	Graph temp;
	IndexMap indices = get(vertex_index, g);

	int x_color, y_color = 0;
	for (std::vector<int>::iterator xi = x_colors.begin();
			xi != x_colors.end() && !valid_coloring; ++xi) {
		x_color = *xi;
		for (std::vector<int>::iterator yi = y_colors.begin();
				yi != y_colors.end() && !valid_coloring; ++yi) {
			y_color = *yi;
			if (y_color != x_color) {
				if (debug) {
					std::cout << token << ": " << "Coloring " << x << " with "
							<< x_color << " and " << y << " with " << y_color
							<< endl;
				}
				// update map
				if (color_map.find(x) != color_map.end())
					color_map[x] = x_color;
				else
					color_map.insert(std::pair<int, int>(x, x_color));

				if (color_map.find(y) != color_map.end())
					color_map[y] = y_color;
				else
					color_map.insert(std::pair<int, int>(y, y_color));

				temp.clear();
				copy_graph(g, temp);

				// add an edge between x and y
				boost::add_edge(get(indices, x), get(indices, y), temp);
				valid_coloring = valid_coloring
						|| ThreeColor(temp, color_map,
								token + "-D(" + std::to_string(x) + ","
										+ std::to_string(y) + ")",
								_index_mapping, _final_color_map);
				if (debug) {
					std::cout << token << ": " << "Valid coloring? "
							<< std::boolalpha << valid_coloring << endl;
				}
			}
		}
	}

	if (valid_coloring) {
		_final_color_map[_index_mapping[x]] = x_color;
		_final_color_map[_index_mapping[y]] = y_color;
	}

	return valid_coloring;
}

template<typename Graph>
void Colorer<Graph>::initIndexMap(int length, map<int, int> &_index_mapping) {
	_index_mapping.clear();
	for (int i = 0; i < length; i++) {
		_index_mapping.insert(std::pair<int, int>(i, i));
	}
}

template<typename Graph>
void Colorer<Graph>::initColorMap(int length, map<int, int> &_final_color_map) {
	_final_color_map.clear();

	for (int i = 0; i < length; i++) {
		_final_color_map.insert(std::pair<int, int>(i, -1));
	}
}
