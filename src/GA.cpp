#include "GA.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define REP(i,n) for (int(i)=0; (i)<(n); (i)++)
#define FOR(i,k,n) for (int(i)=(k); (i)<=(n); (i)++)
#define INF 1e9

const int population_size = 6500;
const int new_population_size = 3500;
const int steps = 2500;

typedef vector<int> vi;
typedef pair<vi,int> pvi;
vector<pvi> population;
vector<pvi> new_population;
vi tmp, tmp2, tmp3;

void _print(vector<int> v) {
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		printf("%d ", *it);
	}
	printf("\n");
}

bool cmp(const pvi &a, const pvi &b) {
	if (a.second < b.second) return 1;
	else return 0;
}

int calculate(vector<int> v, const int &n, int **adjacencyMatrix) {
	int res = 0;
	REP(i,n) {
		res += adjacencyMatrix[v[i]][v[(i+1)%n]];
	}
	return res;
}

void generate(const int &n, int **adjacencyMatrix) {
	population.reserve(population_size);
	new_population.reserve(population_size);
	tmp.reserve(n);
	tmp.clear();
	REP(i,n) tmp.push_back(i);
	REP(i,population_size) {
		population.push_back(make_pair(tmp,calculate(tmp,n,adjacencyMatrix)));
		random_shuffle(population[i].first.begin(), population[i].first.end());
	}
	tmp2.clear();
	tmp3.clear();
	tmp2.reserve(n);
	tmp3.reserve(n);
	REP(i,n) {
		tmp2.push_back(i);
		tmp3.push_back(i);
	}
}

void inversion_mutation(vi &v, const int &n) {
	int l = rand() % n;
	int r = l + rand() % (n-l);
	
	reverse(v.begin()+l, v.begin()+r+1);
	
}

void swap_mutation(vi &v, const int &n) {
	int l = rand() % n;
	int r = rand() % n;
	swap(v[l], v[r]);
}

void displacement_mutation(vi &v, const int &n) {
	int l = rand() % n;
	int r = rand() % n;
	if (r < l) swap(l,r);
	
	int pos = rand() % n;
	if (pos < l || pos > r) {
		vi::iterator itl = v.begin()+l;
		vi::iterator itr = v.begin()+r;
	
		vi tmpp(itl, itr);
		reverse(tmpp.begin(), tmpp.end());
		v.erase(itl, itr);
		if (pos > r) pos = pos - r + l - 1;
		for (vi::iterator it = tmpp.begin(); it != tmpp.end(); it++) {
			v.insert(v.begin()+pos, *it);
		}
	}	
}

vi OX(vi &p1, vi &p2, const int &n) {
	vi res;
	res.resize(n);
	
	int l = rand() % n;
	int r = l + rand() % (n-l);
	
	bool *taken = new bool[n];
	memset(taken,0,n*sizeof(bool));
	
	FOR(i,l,r) {
		res[i] = p1[i];
		taken[res[i]] = true;
	}
	
	int pos = (r+1)%n;
	FOR(k,r+1,n+l-1) {
		while (taken[p2[pos%n]]) pos++;
		res[k%n] = p2[pos++%n];
	}
	
	delete [] taken;

	return res;
}

vi bestpopulation;
int bestresult;

void selection(const int &n, int **adjacencyMatrix) {
	sort(population.begin(), population.end(), cmp);

	if (population[0].second < bestresult) {
		bestpopulation = population[0].first;
		bestresult = population[0].second;
	}
	
	new_population.clear();

	int count=0;	
	REP(i,population_size) {
		new_population.push_back(population[i]);
		count++;
	}

	while (count < population_size) {
		random_shuffle(population[0].first.begin(), population[0].first.end());
		population[0].second = calculate(population[0].first, n, adjacencyMatrix);
		new_population.push_back(population[0]);
		count++;
	}

	population.clear();
	population = new_population;
}

void mutation(vi &v, const int &n) {
		double prob = (double)rand() / RAND_MAX;
		if (prob > 0.7) inversion_mutation(v, n);
		else if (prob > 0.4) swap_mutation(v, n);
		else displacement_mutation(v, n);
}

void judge(const int &n, int **adjacencyMatrix) {
	REP(i,population_size) population[i].second = calculate(population[i].first, n, adjacencyMatrix);
}

void reproduction(const int &n, int **adjacencyMatrix) {
	for (int j=0; j<new_population_size; j+=2) {
		int a = rand()%population_size;
		int b = rand()%population_size;
		while (a==b) b = rand()%population_size;

		double r1 = (double)rand()/RAND_MAX/10;
		double r2 = (double)rand()/RAND_MAX/10;
		
		vi v1,v2;
		v1.resize(n);
		v2.resize(n);
		REP(i,n) {
			v1[i] = v2[i] = i;
		}
		
		random_shuffle(v1.begin(), v1.end());
		random_shuffle(v2.begin(), v2.end());

		v1 = OX(population[a].first, population[b].first, n);
		v2 = OX(population[b].first, population[a].first, n);			
		
		if ((double)rand()/RAND_MAX <= r1) {
			mutation(v1, n);
		}
		if ((double)rand()/RAND_MAX <= r2) {
			mutation(v2, n);
		}
		population.push_back(make_pair(v1, calculate(v1, n, adjacencyMatrix)));
		population.push_back(make_pair(v2, calculate(v2, n, adjacencyMatrix)));
	}
}

int GA(int **adjacencyMatrix, const int &n, vi &result) {
	srand(time(0));
	generate(n,adjacencyMatrix);	
	
	sort(population.begin(), population.end(), cmp);
	bestpopulation = population[0].first;
	bestresult = population[0].second;

	random_shuffle(population.begin(), population.end());

	REP(s,steps) {
		
		judge(n, adjacencyMatrix);
		
		selection(n, adjacencyMatrix);
		
		reproduction(n, adjacencyMatrix);
	}
	REP(i,(int)population.size()) {
		if (population[i].second < bestresult) {
			bestresult = population[i].second;
			bestpopulation = population[i].first;
		}
	}

	result = bestpopulation; 
	
	return bestresult;
}
