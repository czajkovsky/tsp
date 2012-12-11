#include "simulatedAnnealing.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define REP(i,n) for (int i=0; i<n; i++)

typedef vector<int> vi;

const int steps = 100000;
vi sbest;
int ebest;

int energy(vector<int> v, const int &n, int **adjacencyMatrix) {
        int res = 0;
        REP(i,n) {
                res += adjacencyMatrix[v[i]][v[(i+1)%n]];
        }
        return res;
}

void generate(vi &s, const int &n) {
	s.reserve(n);
	REP(i,n) {
		s.push_back(i);
	}

	random_shuffle(s.begin(), s.end());
}

vi neighbour(vi &s, const int &n) {
	int a = rand() % n;
	int b = a;
	while (a == b) {
		b = rand() % n;
	}
	vi res = s;
	swap(res[a], res[b]);
	return res;
}

double P(int e, int enew, double T) {
	if (enew < e) return 1;
	else return exp((double)(enew-e)/T);
}

int simulatedAnnealing(int **adjacencyMatrix, const int &n, vi &result) {
	srand(time(0));
	vi s;

	generate(s,n);
	int e = energy(s, n, adjacencyMatrix);	

	sbest = s;
	ebest = e;

	vi snew;
	int enew;

	REP(k,steps) {
		double T = (double)k/steps;
		snew = neighbour(s, n);
		enew = energy(snew, n, adjacencyMatrix);
		if (P(e,enew,T) > (double)rand()/RAND_MAX) {
			s = snew;
			e = enew;
		}
		if (enew < ebest) {
			sbest = snew;
			ebest = enew;
		}
	}


	result = sbest;
	return ebest;
}
