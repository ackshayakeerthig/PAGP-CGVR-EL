
#include "SceneGenerator.h"

std::vector<Sphere> generateRandomScene(
    int numberOfSpheres,
    float spaceSize,
    unsigned int seed)
{
    std::vector<Sphere> spheres;

    std::mt19937 generator(seed);

    std::uniform_real_distribution<float> positionDistribution(
        -spaceSize / 2.0f,
        spaceSize / 2.0f
    );

    std::uniform_real_distribution<float> radiusDistribution(
        0.3f,
        1.0f
    );

    for (int i = 0; i < numberOfSpheres; i++)
    {
        Sphere sphere;

        sphere.id = i;

        sphere.x = positionDistribution(generator);
        sphere.y = positionDistribution(generator);
        sphere.z = positionDistribution(generator);

        sphere.radius = radiusDistribution(generator);

        spheres.push_back(sphere);
    }

    return spheres;
}
