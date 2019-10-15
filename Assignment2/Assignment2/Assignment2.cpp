// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <stack> 
#include "Cell.h"
#include <vector>
using namespace std;

Cell grid[10][10];
vector<Cell> openList;
vector<Cell> closedList;

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
	Cell startCell(0, 0);
	startCell.g = 0;
	Cell endCell(9, 9);
	startCell.h = heuristic(startCell, endCell);
	startCell.f = startCell.h + startCell.g;
	openList.push_back(startCell);
	while (openList.size() > 0) {
		Cell currentCell = openList.at(0);
		for (int i = 1; i < openList.size(); i++)
		{
			if (currentCell.f > openList.at(i).f) {
				currentCell = openList.at(i); 
			}
		}

		if (currentCell.col == endCell.col && currentCell.row == endCell.row) {
			tracePath();
		}

		openList.pop_back();
		closedList.push_back(currentCell);

		vector<Cell> neigbours;
		int col = currentCell.col;
		int row = currentCell.row;

		neigbours.push_back(grid[col-1][row]);
		neigbours.push_back(grid[col][row-1]);
		neigbours.push_back(grid[col+1][row]);
		neigbours.push_back(grid[col][row+1]);

		for (Cell neighbour : neigbours)
		{
			bool isInClosedSet = false;
			for (Cell cellInClosedList: closedList) {
				if (neighbour.col == cellInClosedList.col && neighbour.row == neighbour.row) {
					isInClosedSet = true;
					break;
				}
			}

			if (!isInClosedSet) {
				break;
			}

			int score = currentCell.g + 1;

			if (score < neighbour.g) {

			}
		}
	}
}

int heuristic(Cell cell, Cell endCell) {
	int h = (cell.col - endCell.col) + (cell.row - endCell.row);
	return h;
}

int main()
{
	createArea();

	printArea();

	findShortestPath();
}