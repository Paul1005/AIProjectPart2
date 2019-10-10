#pragma once
class Cell {
public:
	Cell() {}
	Cell(int col, int row) : col(col), row(row){}

	int g;
	int h;
	int f;

	char symbol;

	int col;
	int row;
	Cell* parent;
};

