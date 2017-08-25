//
//  Graph.cpp
//  by Risa Toyoshima
//

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Graph.h"

using namespace std;

/*
 Create adjacency list with vetor of list(of pair)
 [adj]
 AU| -> (C)G -> G
 G | -> (G)AC -> AU -> C
 AC| -> (UAUAC)G
 C | -> U
 */

void Graph::addEdge(string u, string v, string e) {
    adj[u].push_back(make_pair(v, e));
}

void Graph::printAllPaths(string s, string d) {
    map<string, int> visited;

    cout << "yay Im here! s: "<< s << " d: " << d << endl;
    
    map<int, string> path;
    int path_index = 0; // Initialize path[] as empty
    
    // Call the recursive helper function to print all paths
    printAllPathsUtil(s, d, "", visited, path, path_index);
}

void Graph::printAllPathsUtil(string u, string d, string e, map<string, int> visited, map<int, string> path, int &path_index) {
    string verEdge = e + u;
    string pathTemp;
    
    // Mark the current node and store it in path[]
    
    if (!visited.count(verEdge))  //if the vertex has never visited yet, set to 0
        visited[verEdge] = 0;
    else  //if the vertex has been visited, increment by 1
        visited[verEdge]++;
    
    path[path_index] = verEdge;
    path_index++;
    
    map<string, list<pair<string, string>>>::iterator it1;
    list<pair<string, string>>::iterator it2;
    if (u == d) {
        for (int i = 0; i < path_index; i++) {
            pathTemp += path[i];
            if (pathMax <= path_index)
                pathMax = path_index;
        }
        if (pathMax == path_index)
            cout << pathTemp << endl;
        pathTemp.clear();
    }
    else {
        for (it1 = adj.begin(); it1 != adj.end(); it1++) {
            if (it1 -> first == u) {
                for (it2 = adj[it1 -> first].begin(); it2 != adj[it1 -> first].end(); it2++) {
                    string tmp = it2 -> second + it2 -> first;
                    if (!visited[tmp])
                        printAllPathsUtil(it2 -> first, d, it2 -> second, visited, path, path_index);
                }
            }
        }
    }
    
    
    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[verEdge]--;
}





