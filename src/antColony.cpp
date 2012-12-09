#include "antColony.h"
#include <cstring>
#include <cstdio>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <math.h>

#define INF 1e9
/* best results for numOfAnts = number of cities */
const int numOfAnts = 12; 
const int steps = 100;
/* used in counting the probability of selecting edge */
const double alfa = 1;
const double beta = 3; 
/* longest possible edge (used in initation of pheromons -> initPheromon) */
const int longestEdge = 100;
const double evaporation = 0.5;
const double q=1;

int bestres=INF;
vector<int> bestresdet;

struct ant {
	int curCity;
	int startCity;
	int cycleLength;
	vector<int> visitedCities;
	set<int> unVisitedCities;
};

void initAnts(ant ants[],const int &n) {
	for(int i=0; i<numOfAnts; i++) {
		ants[i].visitedCities.clear();
		ants[i].cycleLength=0;
		ants[i].startCity=rand()%n;
		ants[i].curCity=ants[i].startCity;
		for(int j=0; j<n; j++) ants[i].unVisitedCities.insert(j);
		ants[i].visitedCities.push_back(ants[i].startCity);
		ants[i].unVisitedCities.erase(ants[i].startCity);
	}
}

void printPheromon(double **pheromon,const int &n) {
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			printf("%f ",pheromon[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void initPheromon(double **pheromon, double **deltaPheromon, int **adjacencyMatrix, int n) {
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			deltaPheromon[i][j]=0;
			if(i==j) pheromon[i][j]=1/double(INF);
			else pheromon[i][j]=1/double(adjacencyMatrix[i][j]+longestEdge);
		}
	}
}
void closingCycle(ant &curAnt, double **deltaPheromon, int **adjacencyMatrix) {
	curAnt.cycleLength+=adjacencyMatrix[curAnt.curCity][curAnt.startCity];
	if(curAnt.cycleLength<bestres) {
		bestres=curAnt.cycleLength;
		bestresdet=curAnt.visitedCities;
	}
	curAnt.visitedCities.push_back(curAnt.startCity);
	vector<int>::iterator it;
	for(it=curAnt.visitedCities.begin(); it!=curAnt.visitedCities.end()-1; it++) {
		deltaPheromon[(*it)][*(it+1)]+=q/double(curAnt.cycleLength);
	}
}
void nextCity(ant &curAnt, double **pheromon, int **adjacencyMatrix) {
	double highestProbability=-1,p;
	int selectedCity=-1;
	int curCity=curAnt.curCity;
	double pheromonSum=0;
	set<int>::iterator it;
	for(it=curAnt.unVisitedCities.begin(); it!=curAnt.unVisitedCities.end(); it++) {
		if(*it!=curCity) pheromonSum+=pow(pheromon[curCity][(*it)],alfa)*pow(1/double(adjacencyMatrix[curCity][*it]),beta);
	}
	for(it=curAnt.unVisitedCities.begin(); it!=curAnt.unVisitedCities.end(); it++) {
		if(*it!=curCity) {
			p=pow(pheromon[curCity][(*it)],alfa)*pow(1/double(adjacencyMatrix[curCity][*it]),beta)/pheromonSum;
			if(p>highestProbability) {
				selectedCity=*it;
				highestProbability=p;
			}
		}
	}
	curAnt.cycleLength+=adjacencyMatrix[curCity][selectedCity];
	curAnt.unVisitedCities.erase(selectedCity);
	curAnt.visitedCities.push_back(selectedCity);
	curAnt.curCity=selectedCity;
}

void updatePheromon(double **pheromon, double **deltaPheromon, const int &n) {
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			pheromon[i][j]=pheromon[i][j]*evaporation+deltaPheromon[i][j];
			deltaPheromon[i][j]=0;
		}
	}

}

void print(ant ants[]) {
	for(int i=0; i<numOfAnts; i++) {
		printf("Ant (%d): \t%d\t",i,ants[i].cycleLength);
		vector<int>::iterator it;
		for(it=ants[i].visitedCities.begin(); it!=ants[i].visitedCities.end(); it++) {
			printf("%d ",*it);
		}
		printf("\n");
	}
}

int antColony(int **adjacencyMatrix, const int &n, vector<int> &result) {

	int res=0;

	srand(time(0));
	result.clear();
	result.reserve(n);

	ant ants[numOfAnts];

	double **pheromon = new double*[n];
	double **deltaPheromon = new double*[n];
	for (int i=0; i<n; i++) {
		pheromon[i] = new double[n];
		deltaPheromon[i] = new double[n];
	}

	initPheromon(pheromon, deltaPheromon, adjacencyMatrix, n);
	
	/* main loop */
	for (int i=0; i<steps; i++) {
		initAnts(ants,n);
		for(int j=0; j<numOfAnts; j++) {
			for(int k=0; k<n-1; k++) {
				nextCity(ants[j],pheromon,adjacencyMatrix);
			}
			/* last move + update deltaPheromon */
			closingCycle(ants[j],deltaPheromon,adjacencyMatrix);
		}
		updatePheromon(pheromon, deltaPheromon, n);
		
	}
	result=bestresdet;
	return bestres;
}
