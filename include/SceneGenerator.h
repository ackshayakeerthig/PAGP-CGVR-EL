
#ifndef SCENE_GENERATOR_H
#define SCENE_GENERATOR_H

#include <vector>
#include <random>

#include "Sphere.h"

std::vector<Sphere> generateRandomScene(
    int numberOfSpheres,
    float spaceSize,
    unsigned int seed
);

#endif
