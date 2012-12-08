#include "MST_approx.h"
#include <cstdio>
#include <algorithm>

#define INF 1e9

typedef pair<int,int> pi;
typedef vector<pi> vp;

bool cmp(const pi &a, const pi &b) {
	if (a.first >= b.first) return 1;
	else return 0;
}

void MST_PRIM(int **adjacencyMatrix, const int &n) {
	const int r = 0;
	int *key = new int[n];
	int *prev = new int[n];
	bool *inQ = new bool[n];
	
	for (int i=0; i<n; i++) {
		key[i] = INF;
		prev[i] = 0;
		inQ[i] = true;
	}
	key[r] = 0;
	
	vp Q;
	Q.reserve(n);
	for (int i=0; i<n; i++) {
		Q.push_back(make_pair(key[i],i));
	}
	make_heap(Q.begin(), Q.end(), cmp);
	
	while (!Q.empty()) {
		pop_heap(Q.begin(), Q.end(), cmp);
		pi tmp = Q.back();
		Q.pop_back();
		int u = tmp.first;
		inQ[u] = false;
		for (int v=0; v<n; v++) {
			if (v != u && inQ[v] && adjacencyMatrix[u][v] < key[v]) {
				prev[v] = u;
				key[v] = adjacencyMatrix[u][v];
				
			}
		}
	}
	
}

int MST_approx(int **adjacencyMatrix, const int &n, vector<int> &result) {
	int res = 0;
	
	
	
	return res;
}
