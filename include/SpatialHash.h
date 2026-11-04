#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <vector>
#include <utility>

#include "Sphere.h"

std::vector<std::pair<int, int>> spatialHashCollisionDetection(
    const std::vector<Sphere>& spheres,
    float cellSize
);

#endif