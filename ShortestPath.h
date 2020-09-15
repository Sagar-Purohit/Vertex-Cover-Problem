#ifndef ECE650_A2_SHORTESTPATH_H
#define ECE650_A2_SHORTESTPATH_H
#include <bits/stdc++.h>


//int currentNode(const int V, const int min_dst[], const bool visited_vertex[]);
void add_edge(std::vector<int> adj[], int src, int dest);
bool BFS(std::vector<int> adj[], int src, int dest, int v, int pred[], int dist[]);
void printShortestDistance(std::vector<int> adj[], int s, int dest, int v);



#endif //ECE650_A2_SHORTESTPATH_H