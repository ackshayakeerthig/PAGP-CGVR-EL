
#include "UniformGrid.h"
#include "Collision.h"

#include <map>
#include <tuple>
#include <set>
#include <cmath>
#include <algorithm>

std::vector<std::pair<int, int>> uniformGridCollisionDetection(
    const std::vector<Sphere>& spheres,
    float cellSize)
{
    std::vector<std::pair<int, int>> collisions;

    if (cellSize <= 0.0f)
    {
        return collisions;
    }

    // Each grid cell stores the indices of spheres overlapping it.
    std::map<std::tuple<int, int, int>, std::vector<int>> grid;

    // Assign each sphere to every cell overlapped by its bounding box.
    for (int i = 0; i < static_cast<int>(spheres.size()); i++)
    {
        const Sphere& sphere = spheres[i];

        int minX = static_cast<int>(std::floor(
            (sphere.x - sphere.radius) / cellSize));

        int maxX = static_cast<int>(std::floor(
            (sphere.x + sphere.radius) / cellSize));

        int minY = static_cast<int>(std::floor(
            (sphere.y - sphere.radius) / cellSize));

        int maxY = static_cast<int>(std::floor(
            (sphere.y + sphere.radius) / cellSize));

        int minZ = static_cast<int>(std::floor(
            (sphere.z - sphere.radius) / cellSize));

        int maxZ = static_cast<int>(std::floor(
            (sphere.z + sphere.radius) / cellSize));

        for (int x = minX; x <= maxX; x++)
        {
            for (int y = minY; y <= maxY; y++)
            {
                for (int z = minZ; z <= maxZ; z++)
                {
                    grid[{x, y, z}].push_back(i);
                }
            }
        }
    }

    // Prevent duplicate collision pairs.
    std::set<std::pair<int, int>> uniquePairs;

    // Compare spheres belonging to the same cell.
    for (const auto& cell : grid)
    {
        const auto& objects = cell.second;

        for (int i = 0; i < static_cast<int>(objects.size()); i++)
        {
            for (int j = i + 1; j < static_cast<int>(objects.size()); j++)
            {
                const Sphere& a = spheres[objects[i]];
                const Sphere& b = spheres[objects[j]];

                if (checkCollision(a, b))
                {
                    int firstID = std::min(a.id, b.id);
                    int secondID = std::max(a.id, b.id);

                    uniquePairs.insert({firstID, secondID});
                }
            }
        }
    }

    collisions.assign(uniquePairs.begin(), uniquePairs.end());

    return collisions;
}
