// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <stack> 
#include "Cell.h"
using namespace std;

Cell grid[10][10];
stack <pair<int, int>> openList;
stack <pair<int, int>> closedList;

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
			Cell newCell(i, j);
			for (int k = 0; k < 22; k++) {
				if (i == blockedCells[k].first && j == blockedCells[k].second) {
					newCell.symbol = '#';
					break;
				}
				else {
					newCell.symbol = ' ';
				}
			}
			grid[i][j] = newCell;
		}
	}
}

void createStartPoint() {
	Cell startCell(0, 0);
	startCell.symbol = 'S';
	grid[0][0] = startCell;
}

void createEndPoint() {
	Cell endCell(9, 9);
	endCell.symbol = 'E';
	grid[9][9] = endCell;
}

void createArea() {
	generateGrid();
	createStartPoint();
	createEndPoint();
}

void printArea() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << grid[i][j].symbol;
		}
		cout << '\n';
	}
}

void findShortestPath() {
	/*int h = (cell.x - goal.x) + (cell.y - goal.y);
	int g = distancetravelled;
	int f = h + g;*/

	openList.push(make_pair(0,0));
	while (openList.size() > 0) {

	}
}

int main()
{
	createArea();

	printArea();

	//findShortestPath();
}