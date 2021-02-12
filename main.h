#pragma once

#include <vector> // C++11 array

const static unsigned int BASE_GRID_SIZE = 20; //Making this a global constant to avoid array issues.

typedef std::vector<std::vector<bool> > Grid;

void printGrid(Grid* grid, unsigned int window);
// void reserve_space(Grid* grid, size_t size);

void update(Grid** current, size_t window);
size_t neighbors(Grid* grid, unsigned int x, unsigned int y, size_t window);
bool rule_gol(bool live, size_t num_neighbors);
bool rule_recurse1(bool live, size_t num_neighbors, Grid** grid, size_t window);
bool rule_recurse2(bool live, size_t num_neighbors, Grid** grid, size_t window);
