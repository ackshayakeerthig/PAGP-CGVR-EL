
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>

#include "Sphere.h"
#include "Collision.h"
#include "SceneGenerator.h"
#include "UniformGrid.h"
#include "SpatialHash.h"
#include "SweepAndPrune.h"


std::vector<std::pair<int, int>> bruteForceCollisionDetection(
    const std::vector<Sphere>& spheres);

    
// Stores the results of one algorithm's benchmark.
struct BenchmarkResult
{
    double averageTime;
    double standardDeviation;

    std::vector<std::pair<int, int>> collisions;
};

// Reusable benchmark function.
// It works with any of our four collision detection algorithms.
template <typename Algorithm>
BenchmarkResult benchmarkAlgorithm(
    const std::vector<Sphere>& spheres,
    int repetitions,
    Algorithm algorithm)
{
    std::vector<double> times;

    std::vector<std::pair<int, int>> collisions;

    // Execute the algorithm multiple times.
    for (int i = 0; i < repetitions; i++)
    {
        auto start = std::chrono::steady_clock::now();

        collisions = algorithm(spheres);

        auto end = std::chrono::steady_clock::now();

        double timeTaken =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        times.push_back(timeTaken);
    }

    // Calculate average execution time.
    double average =
        std::accumulate(times.begin(), times.end(), 0.0)
        / repetitions;

    // Calculate variance.
    double variance = 0.0;

    for (double time : times)
    {
        variance += (time - average) * (time - average);
    }

    variance /= repetitions;

    // Calculate standard deviation.
    double standardDeviation = std::sqrt(variance);

    return {
        average,
        standardDeviation,
        collisions
    };
}

// Runs all four algorithms for one scene size.
void runBenchmark(int numberOfSpheres, int repetitions)
{
    const float spaceSize = 100.0f;
    const float cellSize = 16.0f;
    const unsigned int seed = 42;

    std::cout << "\n\n========================================";
    std::cout << "\nSCENE SIZE: " << numberOfSpheres << " SPHERES";
    std::cout << "\nREPETITIONS: " << repetitions;
    std::cout << "\n========================================\n";

    // Generate scene.
    std::vector<Sphere> spheres = generateRandomScene(
        numberOfSpheres,
        spaceSize,
        seed
    );

    std::cout << "Scene generated successfully!\n";

    // 1. Brute Force
    BenchmarkResult brute = benchmarkAlgorithm(
        spheres,
        repetitions,
        [](const std::vector<Sphere>& s)
        {
            return bruteForceCollisionDetection(s);
        }
    );

    // 2. Uniform Grid
    BenchmarkResult grid = benchmarkAlgorithm(
        spheres,
        repetitions,
        [cellSize](const std::vector<Sphere>& s)
        {
            return uniformGridCollisionDetection(s, cellSize);
        }
    );

    // 3. Spatial Hashing
    BenchmarkResult hash = benchmarkAlgorithm(
        spheres,
        repetitions,
        [cellSize](const std::vector<Sphere>& s)
        {
            return spatialHashCollisionDetection(s, cellSize);
        }
    );

    // 4. Sweep-and-Prune
    BenchmarkResult sap = benchmarkAlgorithm(
        spheres,
        repetitions,
        [](const std::vector<Sphere>& s)
        {
            return sweepAndPruneCollisionDetection(s);
        }
    );

    // Sort collision pairs before correctness comparison.
    std::sort(
        brute.collisions.begin(),
        brute.collisions.end()
    );

    std::sort(
        grid.collisions.begin(),
        grid.collisions.end()
    );

    std::sort(
        hash.collisions.begin(),
        hash.collisions.end()
    );

    std::sort(
        sap.collisions.begin(),
        sap.collisions.end()
    );

    // Correctness validation.
    bool gridMatch = brute.collisions == grid.collisions;
    bool hashMatch = brute.collisions == hash.collisions;
    bool sapMatch = brute.collisions == sap.collisions;

    // Display benchmark results.
    std::cout << std::fixed << std::setprecision(3);

    std::cout << "\n--- BENCHMARK RESULTS ---\n";

    std::cout << std::left
              << std::setw(20) << "Algorithm"
              << std::setw(18) << "Average (ms)"
              << std::setw(18) << "Std Dev (ms)"
              << "Speedup\n";

    std::cout << "----------------------------------------------------------\n";

    std::cout << std::left
              << std::setw(20) << "Brute Force"
              << std::setw(18) << brute.averageTime
              << std::setw(18) << brute.standardDeviation
              << "1.00x\n";

    std::cout << std::left
              << std::setw(20) << "Uniform Grid"
              << std::setw(18) << grid.averageTime
              << std::setw(18) << grid.standardDeviation
              << brute.averageTime / grid.averageTime << "x\n";

    std::cout << std::left
              << std::setw(20) << "Spatial Hashing"
              << std::setw(18) << hash.averageTime
              << std::setw(18) << hash.standardDeviation
              << brute.averageTime / hash.averageTime << "x\n";

    std::cout << std::left
              << std::setw(20) << "Sweep-and-Prune"
              << std::setw(18) << sap.averageTime
              << std::setw(18) << sap.standardDeviation
              << brute.averageTime / sap.averageTime << "x\n";

    // Display collision counts.
    std::cout << "\n--- COLLISION COUNTS ---\n";

    std::cout << "Brute Force: "
              << brute.collisions.size() << "\n";

    std::cout << "Uniform Grid: "
              << grid.collisions.size() << "\n";

    std::cout << "Spatial Hashing: "
              << hash.collisions.size() << "\n";

    std::cout << "Sweep-and-Prune: "
              << sap.collisions.size() << "\n";

    // Display correctness.
    std::cout << "\n--- CORRECTNESS CHECK ---\n";

    std::cout << "Uniform Grid: "
              << (gridMatch ? "PASS" : "FAIL") << "\n";

    std::cout << "Spatial Hashing: "
              << (hashMatch ? "PASS" : "FAIL") << "\n";

    std::cout << "Sweep-and-Prune: "
              << (sapMatch ? "PASS" : "FAIL") << "\n";

    if (gridMatch && hashMatch && sapMatch)
    {
        std::cout << "All algorithms detected identical collisions.\n";
    }
    else
    {
        std::cout << "WARNING: Collision results do not match!\n";
    }
}

int main()
{
    // Each entry contains:
    // {number of spheres, number of repetitions}
    const std::vector<std::pair<int, int>> testConfigurations =
    {
        {1000, 20},
        {2000, 20},
        {5000, 10},
        {10000, 5}
    };

    std::cout << "========================================\n";
    std::cout << "SCENE-ADAPTIVE COLLISION DETECTION\n";
    std::cout << "MULTI-SCENE BENCHMARK\n";
    std::cout << "========================================\n";

    // Automatically test every scene configuration.
    for (const auto& configuration : testConfigurations)
    {
        int numberOfSpheres = configuration.first;
        int repetitions = configuration.second;

        runBenchmark(numberOfSpheres, repetitions);
    }

    std::cout << "\n\n========================================\n";
    std::cout << "ALL BENCHMARKS COMPLETED!\n";
    std::cout << "========================================\n";

    return 0;
}
