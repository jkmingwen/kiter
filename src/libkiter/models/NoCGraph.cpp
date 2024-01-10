/*
 * NoCGraph.cpp
 *
 *  Created on: Aug 22, 2019
 *      Author: toky
 */


#include <models/NoCGraph.h>
#include <climits>
#include <algorithm>
#include <fstream>

NoCGraph::NoCGraph(int V) {
	this->const_length = 0;
	this->V = V;
	adj.resize(V);
	dumpPaths = false;
	MESH_SIZE = (V/2);

	//Read from file to set up hardware limit, default is set to FOUR
	HWLIMIT = 4;
}

void NoCGraph::addEdge(int u, int v)
{
	if(u < 0 || v < 0 || u > V*V || v > V*V)
	{
		std::cout << "u=" << u << ",v=" << v << "\n";
		exit(0);
	}
	adj[u].push_back(v); // Add v to u’s list.
}

void NoCGraph::generateAllShortestPaths()
{
	int mesh_size = V/2;
	dumpPaths = true;

	for(int s = 0; s < mesh_size; s++)
	{
		for(int d = 0; d < mesh_size; d++)
		{
			// Mark all the vertices as not visited
			bool *visited = new bool[V];

			// Create an array to store paths
			int *path = new int[V];
			int path_index = 0; // Initialize path[] as empty

			// Initialize all vertices as not visited
			for (int i = 0; i < V; i++)
				visited[i] = false;

			const_length = getPathLength(s, d);
			int myindex = getMapIndex(s, d);

			if (s == d)
			{
				std::vector< std::vector<int> > mypath_vec;
				paths[myindex] = mypath_vec;
			}
			else
			{
				// Call the recursive helper function to print all paths
				printAllPathsUtil(s+MESH_SIZE, d+MESH_SIZE, visited, path, path_index);
			}
			//std::cout << "s=" << s << ",d=" << d << ",len=" << const_length << ",paths=" << paths[myindex].size()  << "\n";
		}
	}
	dumpPaths = false;
}

// Prints all paths from 's' to 'd'
void NoCGraph::printAllPaths(int s, int d, int length)
{
	// Mark all the vertices as not visited
	bool *visited = new bool[V];

	// Create an array to store paths
	int *path = new int[V];
	int path_index = 0; // Initialize path[] as empty

	// Initialize all vertices as not visited
	for (int i = 0; i < V; i++)
		visited[i] = false;

	std::cout << "here start:" << s << "," << d << ",len=" << length << "\n";
	const_length = length;

	// Call the recursive helper function to print all paths
	if (s != d)
		printAllPathsUtil(s, d, visited, path, path_index);
	std::cout << "end\n";
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
void NoCGraph::printAllPathsUtil(int u, int d, bool visited[], int path[], int &path_index)
{
	// Mark the current node and store it in path[]
	visited[u] = true;
	path[path_index] = u;
	path_index++;

	// If current vertex is same as destination, then print current path[]
	if (u == d)
	{
		if(dumpPaths && path[0] != d)
		{
			int myindex = getMapIndex(path[0]-MESH_SIZE, d-MESH_SIZE);
			std::vector<int> mypath;
			for (int i = 0; i<path_index; i++)
				mypath.push_back(path[i]);
			paths[myindex].push_back(mypath);
		}
	}
	else // If current vertex is not destination
	{
		if( (path_index-1) >= const_length)
		{
			visited[u] = false;
			path_index--;
			return;
		}
		// Recur for all the vertices adjacent to current vertex
		for (auto i : adj[u])
			if (!visited[i])
			{
				printAllPathsUtil(i, d, visited, path, path_index);
			}
	}
	// Remove current vertex from path[] and mark it as unvisited
	path_index--;
	visited[u] = false;
}


//find the path contention cost
int NoCGraph::findPathCost(std::vector<int>& mypath)
{
	int util = 0;
	//std::cout << "mypath_size=" << mypath.size() << "\n";
	for (int i = 0; i < (int)mypath.size()-1; i++)
	{
	//	std::cout << "mypath=" << mypath[i] << "," << mypath[i+1] << "\n";
		util = std::max(util, linkUtil[ getMapIndex(mypath[i], mypath[i+1]) ]);
	}
	//std::cout << "util=" << util << "\n";
	return util;
}


// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
void NoCGraph::findLeastCostPath(int u, int d, bool visited[], int path[], int &path_index)
{
	if(u > V*V)
	{
		std::cout << "u=" << u << ",d=" << d << ",pids=" << path_index << "\n";
		exit(0);
	}

	// Mark the current node and store it in path[]
	visited[u] = true;
	path[path_index] = u;
	path_index++;

	//std::cout << "marking u=" << u << ",path_idx=" << path_index << "\n";

	// If current vertex is same as destination, then print current path[]
	if (u == d)
	{
		if(dumpPaths && path[0] != d)
		{
			std::vector<int> mypath;
			for (int i = 0; i<path_index; i++)
				mypath.push_back(path[i]);
			//std::cout << "before mypath_cost\n";
			int new_util = findPathCost(mypath);
			//std::cout << "before return path\n";
			int curr_util = findPathCost(returnPath);
			//std::cout << "after return path\n";

			if(returnPath.size() == 0)
				returnPath = mypath;
			else if(new_util < curr_util)
				returnPath = mypath;
		}
	}
	else // If current vertex is not destination
	{
		if( (path_index-1) >= const_length)
		{
			visited[u] = false;
			path_index--;
			return;
		}
		// Recur for all the vertices adjacent to current vertex
		for (auto i : adj[u])
			if (!visited[i])
				findLeastCostPath(i, d, visited, path, path_index);
	}
	// Remove current vertex from path[] and mark it as unvisited
	path_index--;
	visited[u] = false;
}

std::vector<int> NoCGraph::findPathDijkstra(int u, int d)
{
	u += MESH_SIZE;
	d += MESH_SIZE;

	// Initialize all vertices as not visited
	std::vector<bool> visited(V, false);
	std::vector<int> prev(V, -1);
	std::vector<int> cost_vec(V, INT_MAX);
	std::vector<int> link_util(V, INT_MAX);

	std::vector<int> path_vec;

	if(u == d)
		return path_vec;

	// Mark the current node and store it in path[]
	cost_vec[u] = 0;

	for(int i = 0; i < V ; i++) {

        // Find the vertex with shortest distance (vid)
		int vid = -1;
		int max_dist = INT_MAX;
		for(int j = 0; j < (int)cost_vec.size(); j++) {
			if(visited[j]) { continue;}
			if(max_dist > cost_vec[j]) {
				max_dist = cost_vec[j];
				vid = j;
			}
		}

        // We reached the destination.
        if (vid == d) break;

        // FIXME What is this case ?
		if(vid < 0) continue;

        // remove from the Q
		visited[vid] = true;

		for (auto j : adj[vid]) {
			auto dst = j;
			if(visited[dst]) continue;

			int alt = cost_vec[vid] + 1;
			int myutil = linkUtil[getMapIndex(vid, dst)];

			if((alt == cost_vec[dst] && myutil < link_util[dst]) || (alt < cost_vec[dst])) {
				cost_vec[dst] = alt;
				prev[dst] = vid;
				link_util[dst] = myutil;
			}
		}
	}

	path_vec.push_back(d);
	for(int i = prev[d]; i != u; i = prev[i])
		path_vec.push_back(i);
	path_vec.push_back(u);
	std::reverse(path_vec.begin(), path_vec.end());

	return path_vec;
}

//this function obtains the least contended path between u and d. In case the hardware limit is reached in terms of slots
//per router, we return an empty list
std::vector<int> NoCGraph::findLowContentionPath(int u, int d)
{
	u += MESH_SIZE;
	d += MESH_SIZE;

	// Initialize all vertices as not visited
	std::vector<bool> visited(V, false);
	std::vector<int> prev(V, -1);
	std::vector<int> cost_vec(V, INT_MAX);
	std::vector<int> link_util(V, INT_MAX);

	std::vector<int> path_vec;

	if(u == d)
		return path_vec;

	// Mark the current node and store it in path[]
	cost_vec[u] = 0;

	for(int i = 0; i < V ; i++)
	{
		int vid = -1;
		int max_dist = INT_MAX;
		for(int j = 0; j < (int)cost_vec.size(); j++)
		{
			if(visited[j])
			{
				continue;
			}
			if(max_dist > cost_vec[j])
			{
				max_dist = cost_vec[j];
				vid = j;
			}
		}

		//std::cout << "marking " <<  vid << " visited\n";
		if(vid < 0)
			continue;

		visited[vid] = true;
		for (auto j : adj[vid])
		{
			auto dst = j;
			int alt = cost_vec[vid] + 1;
			int myutil = linkUtil[getMapIndex(vid, dst)];

			if(myutil > HWLIMIT) continue;

			if(visited[dst]) continue;

			if((alt == cost_vec[dst] && myutil < link_util[dst]) || (alt < cost_vec[dst]))
			{
				cost_vec[dst] = alt;
				prev[dst] = vid;
				link_util[dst] = myutil;
			}
		}
	}

	path_vec.push_back(d);
	for(int i = prev[d]; i != u; i = prev[i])
	{
		path_vec.push_back(i);

		if(i == -1) //means no path was found
		{
			path_vec.clear();
			return path_vec; //In the caller function, we will check for empty list
		}
	}

	path_vec.push_back(u);
	std::reverse(path_vec.begin(), path_vec.end());

	return path_vec;
}

