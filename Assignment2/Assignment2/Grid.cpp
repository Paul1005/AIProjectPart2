#include "Grid.h"
#include <iostream>

Grid::Grid(int sizeX, int sizeY, int startX, int startY, int endX, int endY, std::vector<std::pair<int, int>> blockedCells)
	: sizeX(sizeX)
	, sizeY(sizeY)
	, startX(startX)
	, startY(startY)
	, endX(endX)
	, endY(endY)
{
	// create 2D array
	grid = new Cell* [sizeX];
	for (int i = 0; i < sizeX; ++i) {
		grid[i] = new Cell[sizeY];
	}

	// populate with blocked cells
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
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

	// set start and end pos
	grid[startX][startY].col = startX;
	grid[startX][startY].row = startY;
	grid[startX][startY].symbol = 'S';

	grid[endX][endY].col = endX;
	grid[endX][endY].row = endY;
	grid[endX][endY].symbol = 'E';
}

void Grid::print()
{
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			std::cout << grid[j][i].symbol;
		}
		std::cout << std::endl;
	}
}

int Grid::heuristic(int x, int y)
{
	return abs(x - endX) + abs(y - endY);
}
