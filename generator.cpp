#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

const int max_d = 500;
const int min_d = 100;

int main() {
	srand(time(0));

	int n;
	scanf("%d", &n);

	int **adj = new int*[n];
	for (int i=0; i<n; i++) adj[i] = new int[n];

	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			adj[i][j] = 0;
		}
	}

	for (int i=0; i<n-1; i++) {
		for (int j=i+1; j<n; j++) {
			adj[i][j] = adj[j][i] = (int)((double)rand()/RAND_MAX*(max_d-min_d))+min_d;
		}
	}
	
	printf("%d\n", n);


	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			printf("%d ", adj[i][j]);
		}
		printf("\n");
	}

	return 0;
}
