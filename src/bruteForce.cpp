#include "bruteForce.h"
#include <algorithm>
#include <cstdio>

int calculate(vector<int> v, const int &n, int **adjacencyMatrix, bool &fin) {
	int res = 0;
	bool change = false;
	for (int i=0; i<n; i++) {
		res += adjacencyMatrix[v[i]][v[(i+1)%n]];
		if (v[i] != i) change = true;
	}
	if (!change) fin = true;
	return res;
}

int bruteForce(int **adjacencyMatrix, const int &n, vector<int> &result) {
	result.clear();
	result.reserve(n);
	vector<int> tmp;
	tmp.reserve(n);
	int res=0;
	for (int i=0; i<n; i++) {
		tmp.push_back(i);
		res += adjacencyMatrix[i][(i+1)%n];
		result = tmp;
	}

	bool fin = false;
	while (!fin) {
		next_permutation(tmp.begin(), tmp.end());
		int calc = calculate(tmp, n, adjacencyMatrix, fin);
		if (calc < res) {
			res = calc;
			result = tmp;
		}
	}

	return res;
}
