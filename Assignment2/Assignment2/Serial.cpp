// Assignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Serial.h"
#include <chrono>
#include <iostream>

/*void Serial::findShortestPath(Grid& grid) {

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
					}
				}
			}
		}
	}
}*/
/*float** multiplyMatrices(float matrix1[2][3], float matrix2[3][2]) {
	int const max = 3;
	int const min = 2;
	int const height = min;
	int const width = min;

	float** finalMatrix = new float* [height];

	for (int h = 0; h < height; h++)
	{
		finalMatrix[h] = new float[width];
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float value = 0.0f;
			for (int k = 0; k < max; k++) {
				value += (matrix1[i][k] * matrix2[k][j]);
			}
			finalMatrix[i][j] = value;
		}
	}

	return finalMatrix;
}*/

void multiplyMatrices( const float* matrix1row1,
	 const float* matrix1row2,
	 const float* matrix2col1,
	 const float* matrix2col2,
	 float* finalMatrix) {

	for (int i = 0; i < 3; i++) {
		finalMatrix[0] += (matrix1row1[i] * matrix2col1[i]);

		finalMatrix[1] += (matrix1row1[i] * matrix2col2[i]);

		finalMatrix[2] += (matrix1row2[i] * matrix2col1[i]);

		finalMatrix[3] += (matrix1row2[i] * matrix2col2[i]);
	}
}

long Serial::SerialDemo()
{
	/*std::vector<std::pair<int, int> > blockedCells = {
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

	return std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();*/

	/*float matrix1[2][3] = { {1.0f,2.0f,3.0f},{4.0f,5.0f,6.0f} };
	float matrix2[3][2] = { {7.0f,8.0f},{9.0f,10.0f},{11.0f,12.0f} };

	auto start = std::chrono::steady_clock::now();
	float** finalMatrix = multiplyMatrices(matrix1, matrix2);
	auto finish = std::chrono::steady_clock::now();

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			std::cout << finalMatrix[i][j] << ' ';
		}
		std::cout << std::endl;
	}
	return std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();*/

	float* matrix1row1 = new float[3];
	matrix1row1[0] = 1.0f;
	matrix1row1[1] = 2.0f;
	matrix1row1[2] = 3.0f;
	float* matrix1row2 = new float[3];
	matrix1row2[0] = 4.0f;
	matrix1row2[1] = 5.0f;
	matrix1row2[2] = 6.0f;
	float* matrix2col1 = new float[3];
	matrix2col1[0] = 7.0f;
	matrix2col1[1] = 9.0f;
	matrix2col1[2] = 11.0f;
	float* matrix2col2 = new float[3];
	matrix2col2[0] = 8.0f;
	matrix2col2[1] = 10.0f;
	matrix2col2[2] = 12.0f;
	float* finalMatrix = new float[4];
	finalMatrix[0] = 0.0f;
	finalMatrix[1] = 0.0f;
	finalMatrix[2] = 0.0f;
	finalMatrix[3] = 0.0f;

	auto start = std::chrono::steady_clock::now();
	multiplyMatrices(matrix1row1, matrix1row2, matrix2col1, matrix2col2, finalMatrix);
	auto finish = std::chrono::steady_clock::now();

	// Output (some of) the result buffer
	for (int i = 0; i < 4; i++) {
		std::cout << finalMatrix[i] << ' ';
		if (i == 1) {
			std::cout << std::endl;
		}
	}
	return std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
}
