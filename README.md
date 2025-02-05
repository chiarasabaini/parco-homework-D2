# Introduction to Parallel Computing - Deliverable Final

![Tags](https://badgen.net/badge/icon/%23ParallelComputing%20%23MatrixTransposition/14406F1?icon=https://icons.getbootstrap.com/assets/icons/bookmarks-fill.svg&label&labelColor=FFF) ![Language](https://img.shields.io/badge/Built_with-C-F86424?labelColor=000&logo=c) ![Version](https://badgen.net/badge/Version/02.01/F08C2F?labelColor=000) ![Author](https://badgen.net/badge/Author/Chiara%20S./F23A29?labelColor=000)

---

## Table of Contents

- [Introduction to Parallel Computing - Deliverable 2](#introduction-to-parallel-computing---deliverable-2)
- [Table of Contents](#table-of-contents)
- [Project Structure](#project-structure)
- [How to Reproduce](#how-to-reproduce)
    - [On HPC Cluster](#on-hpc-cluster)
    - [On your local machine](#on-your-local-machine)
    - [Data Analysis](#data-analysis)

---

# Project Structure

```bash
├── docs
│   ├── report.pdf
│   └── res/plots
│       └── *.png
├──inc
│   ├── matrix_operations.h
│   ├── test.h
│   └── utils.h
├── out
│   └── data
│       └── *.csv
│   ├── err.e
│   └── out.o
├── src
│   ├── main.c
│   ├── matrix_operations.c
│   ├── test.c
│   ├── utils.c
│   └── performance_analysis.ipynb
├── .gitignore
├── CMakeLists.txt
├── IPC_homework.pbs
└── README.md
```
---

# How to Reproduce

## On HPC Cluster

### Prerequisites
- Access to the HPC cluster (in this case of the University of Trento)
- SSH client

### Steps
1. Open an ssh session on University of Trento's HPC cluster, in your preferred terminal
```bash
$ ssh <username>@hpc.unitn.it
```
2. Clone the repository
```bash
$ git clone https://github.com/chiarasabaini/parco-homework.git
```
3. Navigate the the repository folder
```bash
$ cd parco-homework/
```
3. Run the following command:
```bash
$ qsub IPC_homework.pbs
```

## On your local machine

### Prerequisites
- CMake
- Make
- C Compiler

### Steps
1. Clone the repository
```bash
$ git clone https://github.com/chiarasabaini/parco-homework.git
```
2. Navigate the the repository folder
```bash
$ cd parco-homework/
```
3. Create a build directory
```bash
$ mkdir build
```
4. Navigate to the build directory
```bash
$ cd build/
```
5. Run the following commands:
```bash
$ cmake ..
$ make
```
6. Run the executable
```bash
$ ./project.exe
```

## Data Analysis
You will find all of the .csv files containing the data inside the `data/` folder.
You can process the data and plot the graphs using the provided Jupyter Notebook in the `src/` folder.

Happy testing!
---

