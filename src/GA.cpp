#include "GA.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define REP(i,n) for (int(i)=0; (i)<(n); (i)++)
#define FOR(i,k,n) for (int(i)=(k); (i)<=(n); (i)++)
#define INF 1e9

const int population_size = 500;
const int steps = 300;

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
	REP(i,population_size) {
		tmp.clear();
		//population[i].reserve(n);
		REP(j,n) {
			tmp.push_back(j);
			//population[i].push_back(j);
		}
		population.push_back(make_pair(tmp,calculate(tmp,n,adjacencyMatrix)));
		random_shuffle(population[i].first.begin(), population[i].first.end());
	}
	
	/*printf("Population:\n");
	REP(i,population_size) {
		_print(population[i].first);
	}*/
	
	tmp.reserve(n);
	tmp2.reserve(n);
	tmp3.reserve(n);
	REP(i,n) {
		tmp.push_back(i);
		tmp2.push_back(i);
		tmp3.push_back(i);
	}
}

void inversion_mutation(vi &v, const int &n) {
	int l = rand() % n;
	int r = l + rand() % (n-l);
	
	//printf("Inversion mutation [%d, %d]\n", l, r);
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

void scramble_mutation(vi &v, const int &n) { // tu gdzieś jest bug powodujący runtime error
	int k = rand() % n;
	random_shuffle(tmp.begin(), tmp.end());
	//printf("Scramble mutation %d: ", k);
	REP(i,k) {
		//printf("%d ", tmp[i]);
		tmp2[i] = tmp[i];
	}
	random_shuffle(tmp2.begin(), tmp2.begin()+k);
	tmp3 = v;
	REP(i,k) {
		//printf("%d->%d ", tmp[i], tmp2[i]);
		v[tmp[i]] = tmp3[tmp2[i]];
	}
	//printf("\n");	
}

/*vi PMX(vi &p1, vi &p2, const int &n) {
	vi res;
	res.resize(n);
	int l = rand() % n;
	int r = l + rand() % (n-l);
	
	bool *taken = new bool[n];
	memset(taken,0,n*sizeof(bool*));
	
	FOR(i,l,r) {
		res[i] = p1[i];
		taken[res[i]] = true;
	}
	
	FOR(i,l,r) {
		if (!taken[p2[i]]) {
			int k = i;
			while (posIn2[p1[k]] >= l && posIn2[pi[k]]
		}	
	}
		
	return res;
}*/

vi adaptation;

double calc_adaptation() {
	double res = 0.0;
	int n = population.size();
	adaptation.resize(n);
	int _max = -1;
	
	REP(i,n) {
		_max = max(_max, population[i].second);
	}
	REP(i,n) {
		adaptation[i] = 2*_max - population[i].second;
		res += adaptation[i];
	}
	return res;
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
	
	//printf("OX crossing %d %d\n", l, r);
	//_print(res);
	
	return res;
}

void crossover(const int &n, int **adjacencyMatrix) {
	int a,b;
	int pop_size = population.size();
	REP(i,population_size-pop_size) {
		a = rand()%pop_size;
		b = rand()%pop_size;
		if (a == b) {b++;b%=pop_size;}
		//printf("a:%d b:%d\n", a, b);
		//_print(population[a].first);
		vi c = OX(population[a].first, population[b].first, n);
		population.push_back(make_pair(c,calculate(c,n,adjacencyMatrix)));
		//c = OX(population[b].first, population[a].first, n);
		//population.push_back(make_pair(c,calculate(c,n,adjacencyMatrix)));
	}
}

void selection(const int &n, int **adjacencyMatrix) { // na razie metoda własna :D (1/4 najlepszych + 1/4 ruletka + nie mogą się powtarzać)	
	sort(population.begin(), population.end(), cmp);
	
	int pop_size = population.size();
	new_population.clear();
	
	bool *taken = new bool[pop_size];
	memset(taken,0,pop_size*sizeof(bool));
	
	int count = 0;
	REP(i,pop_size/16) {
		new_population.push_back(population[i]);
		count++;
	}
	
	random_shuffle(population.begin(), population.end());
	vector<double> dystrybuanta;
	dystrybuanta.resize(pop_size);
	
	double adapt = calc_adaptation();
	dystrybuanta[0] = (double)adaptation[0]/adapt;
	FOR(i,1,pop_size-1) {
		dystrybuanta[i] = dystrybuanta[i-1] + (double)adaptation[i]/adapt;
	}
	
	int pos;
	double prob;
	while (count < pop_size/6) {
		//pos = rand() % pop_size;
		do {
		prob = (double)rand() / RAND_MAX;
		pos = (int)(upper_bound(dystrybuanta.begin(), dystrybuanta.end(), prob) - dystrybuanta.begin());
		} while (taken[pos]);
		
		taken[pos] = true;

		new_population.push_back(population[pos]);
		count++;
	}

	while (count < pop_size/2) {
		random_shuffle(population[pos].first.begin(), population[pos].first.end());
		population[pos].second = calculate(population[pos].first, n, adjacencyMatrix);
		new_population.push_back(population[pos]);
		count++;
	}

	population.clear();
	population.assign(new_population.begin(), new_population.end());
}

void mutation(const int &n, int **adjacencyMatrix) {
	REP(i,population_size) {
		double prob = (double)rand() / RAND_MAX;
		//prob = 0.0;
		if (prob > 0.95) inversion_mutation(population[i].first, n);
		else if (prob > 0.9) swap_mutation(population[i].first, n);
		else if (prob > 0.85) displacement_mutation(population[i].first, n);
		//else if (prob > 0.4) scramble_mutation(population[i].first, n);					
	}
}

void judge(const int &n, int **adjacencyMatrix) {
	REP(i,population_size) population[i].second = calculate(population[i].first, n, adjacencyMatrix);
}

int GA(int **adjacencyMatrix, const int &n, vi &result) {
	srand(time(0));
	generate(n,adjacencyMatrix);	
	
	REP(s,steps) {
		
		judge(n, adjacencyMatrix);
		
		selection(n, adjacencyMatrix);
		
		crossover(n, adjacencyMatrix);
		
		mutation(n, adjacencyMatrix);
		
		/*REP(i,population.size()) {
			printf("i%d: ", i);
			_print(population[i].first);
		}*/		
	}
	int res = INF, where = -1;
	REP(i,population.size()) {
		int calc = calculate(population[i].first,n,adjacencyMatrix);
		if (res > calc) {
			res = calculate(population[i].first,n,adjacencyMatrix);
			where = i;
		}
	}
	
	result = population[where].first;
	
	return res;
}
