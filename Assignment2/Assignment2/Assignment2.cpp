// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
using namespace std;

char grid[10][10];

pair<int, int> blockedCells[22] = { {7, 1},
{2, 2}, {3, 2}, {4, 2}, {5, 2}, {5, 2}, {7, 2},
{2, 2}, {3, 2}, {4, 2}, {5, 2}, {7, 3},
{7, 4},
{2, 5}, {3, 5}, {4, 5}, {6, 5},
{2, 6}, {3, 6}, {4, 6},
{7, 8},
{2, 9} };

void generateGrid() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 22; k++) {
				if (i == blockedCells[k].first && j == blockedCells[k].second) {
					grid[i][j] = '#';
					break;
				}
				else {
					grid[i][j] = ' ';
				}
			}
		}
	}
}

void createStartPoint() {
	grid[0][0] = 'S';
}

void createEndPoint() {
	grid[9][9] = 'E';
}

void createArea() {
	generateGrid();
	createStartPoint();
	createEndPoint();
}

void printArea() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << grid[i][j];
		}
		cout << '\n';
	}
}

void findShortestPath() {

}

int main()
{
	createArea();

	printArea();

	findShortestPath();
}