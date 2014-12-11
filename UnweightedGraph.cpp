/**
 * UnweightedGraph Class
 * Stub written by Sorrachai Yingchareonthawornchai, 29 OCT 2014
 * Completed by [Your Name]
 */
#include "UnweightedGraph.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

UnweightedGraph::UnweightedGraph(const string& t)
{
	 //You may add any construction to your private attribute.
	ifstream in(t);
	if(!in.is_open()) {
		cerr << "cannot open file!" << std::endl;
		exit(EXIT_FAILURE);
	}
	string line;
	while(getline(in,line)) {
		istringstream iss(line);
		int v;
		list<int> list_temp;
		while( iss >> v) {
			list_temp.push_back(v);
		}
		adj_list.push_back(list_temp);
	}
	num_vertex = adj_list.size();
}

void UnweightedGraph::DisplayInfo() const{
	cout << "Adjacency list of the graph \n";
	auto printItem = [](const list<int>& alist) {
		copy(begin(alist),end(alist), ostream_iterator<int>(cout, " "));
		cout << endl;
	};
	for_each(begin(adj_list),end(adj_list), printItem);
}

void UnweightedGraph::IsConnectedRecur(int v, list<int> &visited) const {
	
	for(auto var : adj_list[v]){

		bool another = true;
		for(int i : visited){
			if(i == var) {
				another = false;
				break;
			}
		}
		if( another ){
			visited.push_back(var);
			IsConnectedRecur(var, visited);
		}
	}
}

bool UnweightedGraph::IsConnected() const {
	list<int> v_visited;
	v_visited.push_back(0);
	IsConnectedRecur( 0, v_visited);
	if( (int)v_visited.size() == num_vertex)
		return true;
	else
		return false;
}

bool UnweightedGraph::IsEulerian() const {
	for (int i = 0; i < num_vertex; i++){
		if( (adj_list[i].size() % 2) != 0)
			return false;
	}
	return true;
}

list<int> UnweightedGraph::FindEulerianCycle() const {
	list<int> current_cycle;
	vector<list<int>> g_list = adj_list;

	current_cycle.push_back(0);
	int V = 0, next_v = -1, curr_v = 0;

	int sum = 0;
	for(int i =0; i < num_vertex; i++){
		sum += g_list[i].size();
	}

	while( sum > 0){
		list<int> temp_cycle;
		while(V != next_v) {
			next_v = curr_v;
			curr_v = g_list[next_v].back();
			g_list[next_v].pop_back();
			g_list[curr_v].remove(next_v);
			temp_cycle.push_back(curr_v);
			next_v = curr_v;
		}

		sum = 0;
		for(int i =0; i < num_vertex; i++){
			sum += g_list[i].size();
		}

		current_cycle.splice( (++find(current_cycle.begin(), current_cycle.end(),V)) , temp_cycle);

		if(sum == 0) break;
		for(int i =0; i < num_vertex; i++){
			for(int j : g_list[i])
				if( find(current_cycle.begin(), current_cycle.end(), j) != current_cycle.end() ) {
					V = j;
					break;
				}
		}

	}

	return current_cycle;
}

bool UnweightedGraph::VerifyEulerCycle(const list<int>& cycle) const {
	vector<list<int>> g_list = adj_list;

	for(auto vert = cycle.begin(); vert != (--cycle.end()); vert++){
		auto peek = vert;
		++peek;
		if(find(g_list[*vert].begin(), g_list[*vert].end(),  *peek) != g_list[*vert].end()){
			g_list[*vert].remove(*peek);
			g_list[*peek].remove(*vert);
		} else{
			return false;
		}
			
	}
	int sum = 0;
	for(int i =0; i < num_vertex; i++){
		sum += g_list[i].size();
	}
	if(sum == 0)
		return true;
	else
		return false;
}
