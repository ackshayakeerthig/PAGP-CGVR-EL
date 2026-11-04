# Scene-Adaptive Parallel Collision Detection for Interactive Virtual Environments

A research project for comparing broad-phase collision detection algorithms and developing scene-adaptive, workload-aware parallel processing for interactive virtual environments.

## 1. Project Overview

Collision detection is an important component of interactive virtual environments and physics simulations. As the number of objects increases, checking every possible object pair becomes computationally expensive.

This project investigates different broad-phase collision detection algorithms and studies how their performance changes with scene size and spatial distribution.

The long-term objective is to develop a scene-adaptive system that selects an appropriate collision detection algorithm and distributes computational workloads efficiently, particularly using GPU acceleration.

### Current implementation

The current implementation is a C++ CPU-based benchmark that compares:

1. Brute Force
2. Uniform Grid
3. Spatial Hashing
4. Sweep-and-Prune

All four algorithms are tested on identical generated scenes. Their collision-pair outputs are compared against Brute Force to validate correctness.

## 2. Project Structure

```text
SceneAdaptiveCollisionDetection/
│
├── include/
│   ├── Sphere.h
│   ├── Collision.h
│   ├── SceneGenerator.h
│   ├── UniformGrid.h
│   ├── SpatialHash.h
│   └── SweepAndPrune.h
│
├── src/
│   ├── main.cpp
│   ├── Collision.cpp
│   ├── SceneGenerator.cpp
│   ├── UniformGrid.cpp
│   ├── SpatialHash.cpp
│   └── SweepAndPrune.cpp
│
├── tests/
│
├── data/
│   ├── scenes/
│   └── results/
│
├── cuda/
│
├── openmp/
│
├── visualization/
│
├── scripts/
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

### Directory description

| Directory | Purpose |
|---|---|
| `include/` | C++ header files and declarations |
| `src/` | CPU algorithm implementations and benchmark |
| `tests/` | Planned correctness and edge-case tests |
| `data/scenes/` | Generated scene data |
| `data/results/` | Benchmark output and experimental results |
| `cuda/` | Planned CUDA implementation |
| `openmp/` | Planned OpenMP implementation |
| `visualization/` | Planned 3D visualization |
| `scripts/` | Planned experiment and automation scripts |

## 3. Implemented Algorithms

### 3.1 Brute Force

Checks every possible pair of spheres.

For N objects, the number of possible pairs is:

N(N - 1) / 2

This implementation serves as the reference for correctness validation.

### 3.2 Uniform Grid

Divides the simulation space into grid cells.

Each sphere is assigned to every cell overlapped by its bounding box. Spheres sharing a cell are checked for actual collisions.

### 3.3 Spatial Hashing

Uses a hash table to store objects according to their 3D grid-cell coordinates.

This provides an alternative to the ordered map used by the current Uniform Grid implementation.

### 3.4 Sweep-and-Prune

Projects each sphere's bounding volume onto the X axis, sorts the resulting intervals, and checks overlapping intervals.

The current implementation uses a single-axis X sweep followed by a full 3D sphere collision test.

## 4. Collision Detection and Correctness

The simulation uses spheres as objects.

Each sphere contains:

- Unique object ID
- X coordinate
- Y coordinate
- Z coordinate
- Radius

The narrow-phase collision test checks whether the distance between two sphere centers is less than or equal to the sum of their radii.

The squared-distance comparison is:

```cpp
dx * dx + dy * dy + dz * dz
    <= (radiusA + radiusB) * (radiusA + radiusB)
```

To validate correctness:

1. Brute Force generates the reference collision-pair list.
2. Each optimized algorithm generates its own collision-pair list.
3. The lists are sorted.
4. The complete lists are compared.

This checks the actual collision pairs rather than only comparing collision counts.

## 5. Current Benchmark Configuration

The current benchmark uses:

| Parameter | Value |
|---|---|
| Object type | Spheres |
| Simulation space | 100 × 100 × 100 |
| Sphere radius | Random between 0.3 and 1.0 |
| Random seed | 42 |
| Uniform Grid cell size | 16 |
| Spatial Hash cell size | 16 |
| Benchmark repetitions | Depends on scene size |

### Scene sizes currently tested

| Number of spheres | Repetitions |
|---:|---:|
| 1,000 | 20 |
| 2,000 | 20 |
| 5,000 | 10 |
| 10,000 | 5 |

The benchmark automatically executes all four algorithms for every configured scene size.

Execution time, standard deviation, speedup, collision counts, and correctness results are displayed.

## 6. Preliminary Experimental Results

The following results were obtained during the current development experiments.

Average execution time in milliseconds:

| Spheres | Brute Force | Uniform Grid | Spatial Hashing | Sweep-and-Prune |
|---:|---:|---:|---:|---:|
| 1,000 | 3.609 | 1.161 | 0.761 | 0.372 |
| 2,000 | 14.409 | 2.333 | 1.643 | 1.154 |
| 5,000 | 91.471 | 6.238 | 4.981 | 5.844 |
| 10,000 | 369.531 | 14.149 | 11.972 | 19.694 |

### Preliminary observations

- Sweep-and-Prune performed best for the tested 1,000- and 2,000-sphere scenes.
- Spatial Hashing performed best for the tested 5,000- and 10,000-sphere scenes.
- Brute Force became significantly slower as object count increased.
- All three optimized algorithms passed collision-pair validation against Brute Force for all four tested scenes.

These are preliminary CPU benchmark measurements, not universal performance guarantees. Results can vary with hardware, compiler, build configuration, and system load.

## 7. Development Environment

The current development environment is:

| Component | Technology |
|---|---|
| Operating System | Windows 11 |
| Programming Language | C++ |
| C++ Standard | C++17 |
| Compiler | MinGW-w64 UCRT64 (G++) |
| Build System | CMake |
| IDE | Visual Studio Code |
| Version Control | Git and GitHub |

The current CPU implementation does not require CUDA or an NVIDIA GPU.

## 8. Setup Instructions

### Prerequisites

Install the following:

1. Visual Studio Code
2. MSYS2 with the MinGW-w64 UCRT64 toolchain
3. CMake
4. Git

### Install the C++ compiler

Install MSYS2 from:

https://www.msys2.org/

Open the MSYS2 UCRT64 terminal and execute:

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

Add the following directory to your Windows PATH if MSYS2 is installed in its default location:

```text
C:\msys64\ucrt64\bin
```

Verify the compiler:

```bash
g++ --version
```

### Install CMake

Download CMake from:

https://cmake.org/download/

During installation, enable the option to add CMake to PATH.

Verify:

```bash
cmake --version
```

### Install Git

Download Git from:

https://git-scm.com/downloads

Verify:

```bash
git --version
```

## 9. Build and Run

### Step 1: Clone the repository

```bash
git clone <REPOSITORY_URL>
```

Enter the project directory:

```bash
cd SceneAdaptiveCollisionDetection
```

### Step 2: Configure CMake

For the MinGW-w64 compiler, use:

```bash
cmake -S . -B build -G "MinGW Makefiles"
```

### Step 3: Compile

```bash
cmake --build build --parallel 2
```

### Step 4: Execute

In Windows PowerShell:

```powershell
.\build\collision_sim.exe
```

The program will automatically execute the benchmark configurations for 1,000, 2,000, 5,000, and 10,000 spheres.

It will display execution times, standard deviations, speedups, collision counts, and correctness checks.

### Important

If CMake reports a generator mismatch because an existing build directory was configured differently, remove the old `build` directory and repeat Step 2.

## 10. Project Progress Checklist

### Completed

- [x] Created the project directory structure.
- [x] Configured the C++17 project using CMake.
- [x] Created the Sphere data structure.
- [x] Implemented random sphere scene generation.
- [x] Implemented Brute Force collision detection.
- [x] Implemented CPU Uniform Grid collision detection.
- [x] Implemented CPU Spatial Hashing.
- [x] Implemented CPU Sweep-and-Prune.
- [x] Implemented sphere-sphere narrow-phase collision checking.
- [x] Added collision-pair correctness validation against Brute Force.
- [x] Added execution-time benchmarking.
- [x] Added average execution-time calculation.
- [x] Added standard-deviation calculation.
- [x] Added speedup calculation.
- [x] Automated benchmarking across multiple object counts.
- [x] Tested 1,000, 2,000, 5,000, and 10,000 spheres.
- [x] Verified matching collision-pair results for all tested configurations.

### Pending

#### Scene generation and workload analysis

- [ ] Implement uniformly distributed scene configurations as an explicit scene mode.
- [ ] Implement single-cluster scenes.
- [ ] Implement multiple-cluster scenes.
- [ ] Implement sparse scenes.
- [ ] Implement dynamic scenes with moving objects.
- [ ] Save generated scenes to CSV.
- [ ] Collect candidate-pair counts.
- [ ] Collect average and maximum grid-cell occupancy.
- [ ] Study the effect of different grid cell sizes.

#### CPU parallelization

- [ ] Implement OpenMP parallel processing.
- [ ] Compare sequential and OpenMP implementations.
- [ ] Measure CPU parallel speedup.

#### GPU implementation

- [ ] Set up CUDA in Google Colab.
- [ ] Transfer scene data to the GPU.
- [ ] Implement GPU Uniform Grid construction.
- [ ] Implement GPU candidate-pair generation.
- [ ] Implement GPU narrow-phase collision checks.
- [ ] Validate CUDA results against CPU results.
- [ ] Measure GPU execution and memory-transfer time.

#### Workload-aware GPU scheduling

- [ ] Estimate workload per cell and cell pair.
- [ ] Identify heavily loaded regions.
- [ ] Divide heavy workloads into smaller pair-processing tasks.
- [ ] Implement static GPU task assignment.
- [ ] Implement a dynamic GPU task queue.
- [ ] Compare static and adaptive scheduling.
- [ ] Measure task scheduling overhead.

#### Adaptive algorithm selection

- [ ] Collect scene statistics.
- [ ] Analyze algorithm performance under different scene conditions.
- [ ] Develop a threshold-based algorithm selector.
- [ ] Switch algorithms periodically at runtime.
- [ ] Measure algorithm-switching overhead.

#### Visualization and final evaluation

- [ ] Create an OpenGL 3D viewer.
- [ ] Display spheres and collision highlights.
- [ ] Display grid cells and spatial information.
- [ ] Add simulation controls.
- [ ] Display performance metrics.
- [ ] Run final experiments.
- [ ] Generate comparison graphs.
- [ ] Complete the research report and presentation.

## 11. Future Research Direction

The central research objective is to investigate whether scene characteristics can guide algorithm selection and GPU workload distribution.

The planned adaptive system will consider scene information such as:

- Number of objects
- Spatial density
- Grid-cell occupancy
- Candidate-pair count
- Workload imbalance
- Object movement

The GPU workload distribution component will investigate static assignment against adaptive task scheduling.

The most important planned comparison is:

**Static GPU scheduling versus adaptive GPU scheduling across uniform and clustered scenes.**

## 12. Team Collaboration

This is a two-member research project.

Both team members should understand:

- The collision detection algorithms
- The correctness validation approach
- The benchmark methodology
- The adaptive scheduling strategy
- The experimental results

GitHub is used as the shared source-code repository. Experimental outputs and documentation should also be maintained in the repository.

## 13. Current Project Status

**Current milestone:** CPU broad-phase algorithm implementation and multi-scene-size benchmarking.

**Next milestone:** Scene distribution generation and workload characterization.

**Long-term milestone:** GPU acceleration, adaptive workload scheduling, and scene-adaptive algorithm selection.
