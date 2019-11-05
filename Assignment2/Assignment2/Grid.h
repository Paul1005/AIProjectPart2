#pragma once

#include "Cell.h"
#include <vector>

struct Grid
{
	Grid(int sizeX, int sizeY, int startX, int startY, int endX, int endY, std::vector<std::pair<int, int> > blockedCells);
	~Grid();
	Cell** grid;
	const int sizeX;
	const int sizeY;
	const int startX;
	const int startY;
	const int endX;
	const int endY;
	Cell* operator [] (int x) const { return grid[x]; }
	void print();
	int heuristic(int x, int y);
	void createPath();
};
