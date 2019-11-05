#pragma once

#include "Cell.h"
#include <vector>

struct GridRow
{
	GridRow(Cell* arrayEle) : arr(arrayEle) {}
	Cell* arr;
	
	Cell& operator [] (int y) const { return arr[y]; };
};

struct Grid
{
	Grid(int sizeX, int sizeY, int startX, int startY, int endX, int endY, std::vector<std::pair<int, int>> blockedCells);
	Cell** grid;
	const int sizeX;
	const int sizeY;
	const int startX;
	const int startY;
	const int endX;
	const int endY;
	GridRow operator [] (int x) const { return GridRow(grid[x]); }
	void print();
	int heuristic(int x, int y);
};
