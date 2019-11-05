#pragma once

#include "Grid.h"
#include <utility>

namespace Serial
{
	void createPath(Grid grid, std::pair<int, int> finalCell);
	void findShortestPath(Grid grid);
	long SerialDemo();
};
