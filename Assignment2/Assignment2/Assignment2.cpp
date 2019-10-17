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

void createPath() {
	Cell cell = *grid[9][9].parent;
	cell.parent->symbol = '+';
	while (cell.col != 0 && cell.row != 0) {
		grid[cell.col][cell.row].symbol = '+';
		cell = *cell.parent;
	}
}

int heuristic(Cell cell, Cell endCell) {
	int h = (cell.col - endCell.col) + (cell.row - endCell.row);
	return h;
}

void findShortestPath() {
	grid[0][0].g = 0;
	grid[0][0].h = heuristic(grid[0][0], grid[9][9]);
	grid[0][0].f = grid[0][0].h + grid[0][0].g;
	
	Cell firstCell = Cell(grid[0][0]);
	openList.push_back(firstCell);
	
	while (openList.size() > 0) {
		Cell& currentCell = openList.at(0);
		for (int i = 1; i < openList.size(); i++)
		{
			if (currentCell.f > openList.at(i).f) {
				currentCell = openList.at(i); 
			}
		}

		cout << currentCell.col << "\n";
		cout << currentCell.row << "\n";
		if (currentCell.col == grid[9][9].col && currentCell.row == grid[9][9].row) {
			createPath();
			break;
		}

		openList.pop_back();
		closedList.push_back(move(currentCell));

		vector<Cell> neighbours;
		int col = currentCell.col;
		int row = currentCell.row;

		if (col > 0) {
			Cell& cell = grid[col - 1][row];
			neighbours.push_back(move(cell));
		} 
		if (col < 9) {
			Cell& cell = grid[col + 1][row];
			neighbours.push_back(cell);
		}
		if (row > 0) {
			Cell& cell = grid[col][row - 1];
			neighbours.push_back(cell);
		}
		if (row < 9) {
			Cell& cell = grid[col][row + 1];
			neighbours.push_back(cell);
		}

		for (Cell& neighbour : neighbours)
		{
			bool isInClosedSet = false;
			for (Cell& cellInClosedList: closedList) {
				if (neighbour.col == cellInClosedList.col && neighbour.row == neighbour.row) {
					isInClosedSet = true;
				}
			}

			if (!isInClosedSet) {
				int score = currentCell.g + 1;

				if (score < neighbour.g) {
					neighbour.parent = &currentCell;
					neighbour.g = score;
					neighbour.f = neighbour.g + neighbour.h;
					for (Cell& cellInOpenList : openList) {
						if (neighbour.col != cellInOpenList.col && neighbour.row != cellInOpenList.row) {
							openList.push_back(move(neighbour));
						}
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