#include "SpatialHash.h"
#include "Collision.h"

#include <unordered_map>
#include <tuple>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>

// Hash function for 3D grid coordinates
struct CellHash
{
    std::size_t operator()(
        const std::tuple<int, int, int>& cell) const
    {
        auto [x, y, z] = cell;

        std::size_t h1 = std::hash<int>{}(x);
        std::size_t h2 = std::hash<int>{}(y);
        std::size_t h3 = std::hash<int>{}(z);

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

std::vector<std::pair<int, int>> spatialHashCollisionDetection(
    const std::vector<Sphere>& spheres,
    float cellSize)
{
    std::vector<std::pair<int, int>> collisions;

    if (cellSize <= 0.0f)
    {
        return collisions;
    }

    // Store sphere indices in hashed grid cells.
    std::unordered_map<
        std::tuple<int, int, int>,
        std::vector<int>,
        CellHash
    > hashGrid;

    // Insert each sphere into every cell overlapped by its bounding box.
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
                    hashGrid[{x, y, z}].push_back(i);
                }
            }
        }
    }

    // Prevent duplicate collision pairs.
    std::set<std::pair<int, int>> uniquePairs;

    // Check spheres stored in the same cell.
    for (const auto& cell : hashGrid)
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