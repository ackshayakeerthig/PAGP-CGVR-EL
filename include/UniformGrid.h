
#ifndef UNIFORM_GRID_H
#define UNIFORM_GRID_H

#include <vector>
#include <utility>

#include "Sphere.h"

std::vector<std::pair<int, int>> uniformGridCollisionDetection(
    const std::vector<Sphere>& spheres,
    float cellSize
);

#endif
