// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Serial.h"
#include <chrono>

void Serial::findShortestPath(Grid& grid) {

	std::vector<std::pair<int, int> > openList;
	std::vector<std::pair<int, int> > closedList;

	// add starting point to the open list
	grid[grid.startX][grid.startY].g = 0;
	grid[grid.startX][grid.startY].h = grid.heuristic(grid.startX, grid.startY);
	grid[grid.startX][grid.startY].f = grid[grid.startX][grid.startY].h + grid[grid.startX][grid.startY].g;

	openList.push_back({ grid.startX, grid.startY });

	while (openList.size() > 0) {
		std::pair<int, int> currentCell = openList.at(0);
		int iterator = 0;
		for (int i = 1; i < openList.size(); i++) // see if any in open list have a lower f value
		{
			if (grid[currentCell.first][currentCell.second].f > grid[openList.at(i).first][openList.at(i).second].f) {
				currentCell = openList.at(i);
				iterator = i;
			}
		}

		if (currentCell.first == grid[grid.endX][grid.endY].col && currentCell.second == grid[grid.endX][grid.endY].row) { // if we reach the end
			break; // break the while loop
		}

		openList.erase(openList.begin() + iterator);
		closedList.push_back(currentCell);

		std::vector<std::pair<int, int> > neighbours;
		int col = currentCell.first;
		int row = currentCell.second;

		// find all neighbours, also check to see if we're at an edge
		if (col > 0) {
			neighbours.push_back({ col - 1, row });
		}
		if (col < grid.sizeX - 1) {
			neighbours.push_back({ col + 1, row });
		}
		if (row > 0) {
			neighbours.push_back({ col, row - 1 });
		}
		if (row < grid.sizeY - 1) {
			neighbours.push_back({ col, row + 1 });
		}

		for (std::pair<int, int> neighbour : neighbours) // go through each neighbour 
		{
			bool isInClosedSet = false;
			for (std::pair<int, int> cellInClosedList : closedList) {
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
					grid[neighbour.first][neighbour.second].h = grid.heuristic(neighbour.first, neighbour.second);
					grid[neighbour.first][neighbour.second].f = grid[neighbour.first][neighbour.second].g + grid[neighbour.first][neighbour.second].h;
					bool isInOpenSet = false;
					for (std::pair<int, int> cellInOpenList : openList) {
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

long Serial::SerialDemo()
{
	std::vector<std::pair<int, int> > blockedCells = {
		{4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0},
		{9, 1},
		{9, 2},
		{9, 3},
		{9, 4},
		{9, 5},
		{4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 6}, {9, 6}
	};

	int const x = 13;
	int const y = 8;

	int const startX = 0;
	int const startY = 2;
	int const endX = 12;
	int const endY = 7;

	Grid grid = Grid(x, y, startX, startY, endX, endY, blockedCells);

	auto start = std::chrono::steady_clock::now();
	findShortestPath(grid);
	auto finish = std::chrono::steady_clock::now();

	grid.createPath();
	grid.print();

	return std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
}
