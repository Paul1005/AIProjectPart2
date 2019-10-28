// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <stack> 
#include "Cell.h"
#include <vector>
#include <cmath>    
#include <limits.h>

using namespace std;

Cell grid[10][10];
vector<pair<int, int>> openList;
vector<pair<int, int>> closedList;

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
			grid[i][j].col = i;
			grid[i][j].row = j;
			for (int k = 0; k < 22; k++) {
				if (i == blockedCells[k].first && j == blockedCells[k].second) {
					grid[i][j].symbol = '#';
					break;
				}
				else {
					grid[i][j].symbol = ' ';
				}
			}
		}
	}
}

void createStartPoint() {
	grid[0][0].col = 0;
	grid[0][0].row = 0;
	grid[0][0].symbol = 'S';
}

void createEndPoint() {
	grid[9][9].col = 9;
	grid[9][9].row = 9;
	grid[9][9].symbol = 'E';
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

void createPath(pair<int, int> finalCell) {
	Cell* cellPointer = grid[finalCell.first][finalCell.second].parent;
	Cell cell = *cellPointer;
	while (cell.col != 0 || cell.row != 0) {
		grid[cell.col][cell.row].symbol = '+';
		cellPointer = cell.parent;
		cell = *cellPointer;
	}
}

int heuristic(Cell startCell, Cell endCell) {
	int h = abs(startCell.col - endCell.col) + abs(startCell.row - endCell.row);
	return h;
}

void findShortestPath() {
	grid[0][0].g = 0;
	grid[0][0].h = heuristic(grid[0][0], grid[9][9]);
	grid[0][0].f = grid[0][0].h + grid[0][0].g;

	openList.push_back({0, 0});

	while (openList.size() > 0) {
		pair<int, int> currentCell = openList.at(0);
		int iterator = 0;
		for (int i = 1; i < openList.size(); i++)
		{
			if (grid[currentCell.first][currentCell.second].f > grid[openList.at(i).first][openList.at(i).second].f ) {
				currentCell = openList.at(i);
				iterator = i;
			}
		}

		if (currentCell.first == grid[9][9].col && currentCell.second == grid[9][9].row) {
			createPath(currentCell);
			break;
		}

		openList.erase(openList.begin() + iterator);
		closedList.push_back(currentCell);

		vector<pair<int, int>> neighbours;
		int col = currentCell.first;
		int row = currentCell.second;

		if (col > 0) {
			neighbours.push_back({col - 1, row});
		}
		if (col < 9) {
			neighbours.push_back({col + 1,row});
		}
		if (row > 0) {
			neighbours.push_back({ col, row - 1 });
		}
		if (row < 9) {
			neighbours.push_back({ col, row + 1 });
		}

		for (pair<int, int> neighbour : neighbours)
		{
			bool isInClosedSet = false;
			for (pair<int, int> cellInClosedList : closedList) {
				if (neighbour.first == cellInClosedList.first && neighbour.second == cellInClosedList.second) {
					isInClosedSet = true;
					break;
				}
			}

			if (!isInClosedSet) {
				char symbol = grid[currentCell.first][currentCell.second].symbol;

				int score = grid[currentCell.first][currentCell.second].g + 1;
				if (symbol == '#') {
					score = INT_MAX;
				}

				if (score < grid[neighbour.first][neighbour.second].g) {
					grid[neighbour.first][neighbour.second].parent = &grid[currentCell.first][currentCell.second];
					grid[neighbour.first][neighbour.second].g = score;
					grid[neighbour.first][neighbour.second].h = heuristic(grid[neighbour.first][neighbour.second], grid[9][9]);
					grid[neighbour.first][neighbour.second].f = grid[neighbour.first][neighbour.second].g + grid[neighbour.first][neighbour.second].h;
					bool isInOpenSet = false;
					for (pair<int, int> cellInOpenList : openList) {
						if (neighbour.first == cellInOpenList.first && neighbour.second == cellInOpenList.second) {
							isInOpenSet = true;
							break;
						}
					}
					if (!isInOpenSet) {
						openList.push_back(neighbour);
						//cout << "test";
					}
				}
			}
		}
	}
}

int main()
{
	createArea();

	findShortestPath();

	printArea();
}