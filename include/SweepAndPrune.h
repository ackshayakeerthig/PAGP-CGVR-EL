#ifndef SWEEP_AND_PRUNE_H
#define SWEEP_AND_PRUNE_H

#include <vector>
#include <utility>

#include "Sphere.h"

std::vector<std::pair<int, int>> sweepAndPruneCollisionDetection(
    const std::vector<Sphere>& spheres
);

#endif