#include "farthestInsertion.h"
#include <cstring>
#include <cstdio>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <math.h>
#include <list>
#include <deque>

#define INF 1e9
int visited[100000];
deque<int> tour;

int findFarthestLocal(int **adjacencyMatrix, const int &n, int vertex) {
	int maxdist=-1;
	int farthest=-1;

	for (int j=0; j<n; j++) {
		if(j!=vertex && visited[j]==0 && adjacencyMatrix[vertex][j]>maxdist) {
			farthest=j;
			maxdist=adjacencyMatrix[vertex][j];
		}
	}
	return farthest;
}
int findFarthestGlobal(int **adjacencyMatrix, const int &n) {
	int maxdist=0;
	int farthest=-1,candidate;
	for(deque<int>::iterator it=tour.begin(); it!=tour.end(); it++) {
		candidate=findFarthestLocal(adjacencyMatrix,n,*it);
		if(adjacencyMatrix[*it][candidate]>maxdist && !visited[candidate]) {
			maxdist=adjacencyMatrix[*it][candidate];
			farthest=candidate;
		}
	}
	return farthest;
}
void printMatrix(int **adjacencyMatrix,const int &n) {
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			printf("%d ",adjacencyMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}
void printTour(deque<int> tour) {
	printf("Tour: ");
	for(deque<int>::iterator it=tour.begin(); it!=tour.end(); it++) printf("%d ",*it);
	printf("\n");
}
deque<int>::iterator findClosest(int vertex, int **adjacencyMatrix) {
	int mindist=INF;
	deque<int>::iterator res;
	for(deque<int>::iterator it=tour.begin(); it!=tour.end(); it++) {
		if(adjacencyMatrix[*it][vertex]<mindist) {
			res=it;
			mindist=adjacencyMatrix[*it][vertex];
		}
	}
	return res;
}

int putIntoTour(int length, int **adjacencyMatrix, const int &n, int vertex) {
	tour.insert(findClosest(vertex, adjacencyMatrix),vertex);
	return 0;
}

int farthestInsertion(int **adjacencyMatrix, const int &n, vector<int> &result) {

	srand(time(0));
	result.clear();
	result.reserve(n);

	for(int i=0; i<n+1; i++) visited[i]=0;
	tour.clear();

	int res=-1;

	/* setting first vertex */
	int start=0;
	tour.push_back(start);
	visited[start]=1;
	int current=start;

	/* main loop */
	for(int i=0; i<n-1; i++) {
		current=findFarthestLocal(adjacencyMatrix, n, i);
		visited[current]=1;
		putIntoTour(0, adjacencyMatrix, n, current);
	}

	deque<int>::iterator it;
	int i=0;
	//printTour(tour);
	res=adjacencyMatrix[*(tour.end())][*(tour.begin())];
	for(it=tour.begin(); it!=tour.end(); it++) {
		i++;
		result.push_back(*it);
		if(i>1) res+=adjacencyMatrix[*it][*(it-1)];
	}
	return res;
}