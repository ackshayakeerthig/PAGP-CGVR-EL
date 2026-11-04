
#include <vector>
#include <utility>

#include "Sphere.h"
#include "Collision.h"

bool checkCollision(const Sphere& a, const Sphere& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;

    float distanceSquared = dx * dx + dy * dy + dz * dz;

    float radiusSum = a.radius + b.radius;

    return distanceSquared <= radiusSum * radiusSum;
}

std::vector<std::pair<int, int>> bruteForceCollisionDetection(
    const std::vector<Sphere>& spheres)
{
    std::vector<std::pair<int, int>> collisions;

    int n = static_cast<int>(spheres.size());

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (checkCollision(spheres[i], spheres[j]))
            {
                collisions.push_back({
                    spheres[i].id,
                    spheres[j].id
                });
            }
        }
    }

    return collisions;
}
