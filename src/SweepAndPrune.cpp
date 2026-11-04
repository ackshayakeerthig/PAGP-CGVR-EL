
#include "SweepAndPrune.h"
#include "Collision.h"

#include <vector>
#include <utility>
#include <algorithm>

struct Interval
{
    int sphereIndex;
    float minX;
    float maxX;
};

std::vector<std::pair<int, int>> sweepAndPruneCollisionDetection(
    const std::vector<Sphere>& spheres)
{
    std::vector<std::pair<int, int>> collisions;

    std::vector<Interval> intervals;

    // Create X-axis bounding intervals for every sphere.
    for (int i = 0; i < static_cast<int>(spheres.size()); i++)
    {
        Interval interval;

        interval.sphereIndex = i;
        interval.minX = spheres[i].x - spheres[i].radius;
        interval.maxX = spheres[i].x + spheres[i].radius;

        intervals.push_back(interval);
    }

    // Sort intervals by their minimum X coordinate.
    std::sort(
        intervals.begin(),
        intervals.end(),
        [](const Interval& a, const Interval& b)
        {
            return a.minX < b.minX;
        }
    );

    // Sweep through sorted intervals.
    for (int i = 0; i < static_cast<int>(intervals.size()); i++)
    {
        const Interval& a = intervals[i];

        for (int j = i + 1; j < static_cast<int>(intervals.size()); j++)
        {
            const Interval& b = intervals[j];

            // No later interval can overlap A on the X axis.
            if (b.minX > a.maxX)
            {
                break;
            }

            const Sphere& sphereA = spheres[a.sphereIndex];
            const Sphere& sphereB = spheres[b.sphereIndex];

            // Full 3D sphere collision test.
            if (checkCollision(sphereA, sphereB))
            {
                int firstID = std::min(sphereA.id, sphereB.id);
                int secondID = std::max(sphereA.id, sphereB.id);

                collisions.push_back({firstID, secondID});
            }
        }
    }

    return collisions;
}
