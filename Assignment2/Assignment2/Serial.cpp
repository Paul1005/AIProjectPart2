// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Main.h"
#include "Serial.h"

int const x = 13;
int const y = 8;
Cell grid[x][y];
vector<pair<int, int>> openList;
vector<pair<int, int>> closedList;

vector<pair<int, int>> blockedCells = {
{4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0},
{9, 1},
{9, 2},
{9, 3},
{9, 4},
{9, 5},
{4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 6}, {9, 6} };

int const startX = 0;
int const startY = 2;
int const endX = 12;
int const endY = 7;

void generateGrid() {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			grid[j][i].col = j;
			grid[j][i].row = i;
			for (int k = 0; k < blockedCells.size(); k++) {
				if (j == blockedCells[k].first && i == blockedCells[k].second) {
					grid[j][i].symbol = '#';
					break;
				}
				else {
					grid[j][i].symbol = ' ';
				}
			}
		}
	}
}

void createStartPoint() {
	grid[startX][startY].col = startX;
	grid[startX][startY].row = startY;
	grid[startX][startY].symbol = 'S';
}

void createEndPoint() {
	grid[endX][endY].col = endX;
	grid[endX][endY].row = endY;
	grid[endX][endY].symbol = 'E';
}

void createArea() {
	generateGrid();
	createStartPoint();
	createEndPoint();
}

void printArea() {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			cout << grid[j][i].symbol;
		}
		cout << '\n';
	}
}

void createPath(pair<int, int> finalCell) {
	Cell* cellPointer = grid[finalCell.first][finalCell.second].parent;
	Cell cell = *cellPointer;
	while (cell.col != startX || cell.row != startY) {
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
	// add starting point to the open list
	grid[startX][startY].g = 0;
	grid[startX][startY].h = heuristic(grid[startX][startY], grid[endX][endY]);
	grid[startX][startY].f = grid[startX][startY].h + grid[startX][startY].g;

	openList.push_back({ startX, startY });

	while (openList.size() > 0) {
		pair<int, int> currentCell = openList.at(0);
		int iterator = 0;
		for (int i = 1; i < openList.size(); i++) // see if any in open list have a lower f value
		{
			if (grid[currentCell.first][currentCell.second].f > grid[openList.at(i).first][openList.at(i).second].f) {
				currentCell = openList.at(i);
				iterator = i;
			}
		}

		if (currentCell.first == grid[endX][endY].col && currentCell.second == grid[endX][endY].row) { // if we reach the end
			createPath(currentCell); // draw the path
			break; // break the while loop
		}

		openList.erase(openList.begin() + iterator);
		closedList.push_back(currentCell);

		vector<pair<int, int>> neighbours;
		int col = currentCell.first;
		int row = currentCell.second;

		// find all neighbours, also check to see if we're at an edge
		if (col > 0) {
			neighbours.push_back({ col - 1, row });
		}
		if (col < x - 1) {
			neighbours.push_back({ col + 1, row });
		}
		if (row > 0) {
			neighbours.push_back({ col, row - 1 });
		}
		if (row < y - 1) {
			neighbours.push_back({ col, row + 1 });
		}

		for (pair<int, int> neighbour : neighbours) // go through each neighbour 
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
					score = INT_MAX; // set score high if square is an obstacle
				}

				if (score < grid[neighbour.first][neighbour.second].g) { // if the neighbour's new score is less than the neighbour's old score, update its info
					grid[neighbour.first][neighbour.second].parent = &grid[currentCell.first][currentCell.second];
					grid[neighbour.first][neighbour.second].g = score;
					grid[neighbour.first][neighbour.second].h = heuristic(grid[neighbour.first][neighbour.second], grid[endX][endY]);
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

long SerialDemo()
{
	createArea();

	auto start = std::chrono::steady_clock::now();
	findShortestPath();
	auto finish = std::chrono::steady_clock::now();

	printArea();

	return std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
}
