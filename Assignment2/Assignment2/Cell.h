#pragma once
#include <limits.h>

struct Cell {
	Cell() {
		g = INT_MAX;
		h = INT_MAX;
		f = INT_MAX;
	}
	Cell(int col, int row) : col(col), row(row){
		g = INT_MAX;
		h = INT_MAX;
		f = INT_MAX;
	}
	Cell(const Cell& cell) {
		g = cell.g;
		h = cell.h;
		f = cell.f;
		symbol = cell.symbol;
		
		col = cell.col;
		row = cell.row;
		parent = cell.parent;
	};
	~Cell() {};

	int g;
	int h;
	int f;

	char symbol;

	int col;
	int row;
	Cell* parent;
};
