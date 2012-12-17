#include "simulatedAnnealing.h"
#include "nearestNeighbour.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define REP(i,n) for (int i=0; i<n; i++)

typedef vector<int> vi;

const int steps = 150;
const int L = 8000;

vi sbest;
int ebest;

int energy(vector<int> v, const int &n, int **adjacencyMatrix) {
        int res = 0;
        REP(i,n) {
                res += adjacencyMatrix[v[i]][v[(i+1)%n]];
        }
        return res;
}

void generate(int **adjacencyMatrix, vi &s, const int &n) {
//	nearestNeighbour(adjacencyMatrix, n, s);

	s.clear();
	s.reserve(n);
	REP(i,n) s.push_back(i);
	random_shuffle(s.begin(), s.end());

}

vi neighbour(vi &s, const int &n) {
	int a = rand() % n;
	int b = a;
	while (a == b) {
		b = rand() % n;
	}
	//if (a > b) swap(a,b); // zakomenowana metoda alternatywna - obecna daje trochę lepsze wyniki
	vi res = s;
	//random_shuffle(res.begin()+a, res.begin()+b);
	swap(res[a], res[b]);
	return res;
}

double P(int delta, double T) {
	return exp((double)(-delta)/T);
}

double alfa(double t) {
	const double b = 0.0002005;

	return t/(1+b*t);
}

int simulatedAnnealing2(int **adjacencyMatrix, const int &n, vi &result) {
	srand(time(0));
	vi s;


	generate(adjacencyMatrix,s,n);
	int e = energy(s, n, adjacencyMatrix);	

	sbest = s;
	ebest = e;
	
	vi snew;
	int enew;

	double t = 10000000000000000;

	REP(k,steps) {
		REP(j,L) {
			snew = neighbour(s, n);
			enew = energy(snew, n, adjacencyMatrix);
			if (enew < ebest) {
				ebest = enew;
				sbest = snew;
			}
			int delta = enew - e;
			if (delta < 0) {
				e = enew;
				s = snew;
			}
			else {
				double x = (double)rand() / RAND_MAX;
				if (x < P(delta,t)) {
					s = snew;
					e = enew;
				}
			}
		}
		t = alfa(t);
	}


	result = sbest;
	return ebest;
}

int repets = 7;

int simulatedAnnealing(int **adjacencyMatrix, const int &n, vi &result) {
	int bestres = 1e9;
	while (repets--) {
		vi tmpvec;
		int tmp = simulatedAnnealing2(adjacencyMatrix, n, tmpvec);
		if (bestres > tmp) {
			bestres = tmp;
			result = tmpvec;
		}
	}
	return bestres;
}
