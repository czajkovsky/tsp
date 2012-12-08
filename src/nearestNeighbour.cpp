#include "nearestNeighbour.h"
#include <cstring>
#include <cstdio>

int nearestNeighbour(int **adjacencyMatrix, const int &n, vector<int> &result) {
	int res=0;

	result.clear();
	result.reserve(n);

	int *visited = new int[n];
	memset(visited, 0, n*sizeof(int));
	int minv, minw;

	visited[0] = 1;
	result.push_back(0);

	int pos = 0;

	for (int i=0; i<n-1; i++) {
		minv = 1e9;
		minw = -1;
		for (int j=0; j<n; j++) {
			if (pos != j && !visited[j] && adjacencyMatrix[pos][j] < minv) {
				minv = adjacencyMatrix[pos][j];
				minw = j;
			}
		}
	
		result.push_back(minw);
		visited[minw] = 1;
		res += minv;
		pos = minw;
	}

	res += adjacencyMatrix[minw][0];
	
	delete(visited);
	return res;
}
