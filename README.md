# Introduction to Parallel Computing - Deliverable 2

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

```sh
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

## Dependencies
- GCC - version 9.1.0 or later
    ```sh
    $   module load gcc91
    ```
- OpenMPI or MPICH - version 3.2.1 or later
    ```sh
    $   module load mpich-3.2.1--gcc-9.1.0
    ```
- CMake (for local compilation) [Download CMake](https://cmake.org/download/)
- PBS (for HPC cluster job scheduling, must be installed on the cluster)

## On HPC Cluster

### Prerequisites
- Access to the HPC cluster (in this case of the University of Trento)
- SSH client

### Steps
1. Open an ssh session on University of Trento's HPC cluster, in your preferred terminal
```sh
$ ssh <username>@hpc.unitn.it
```
2. Clone the repository
```sh
$ git clone https://github.com/chiarasabaini/parco-homework-D2.git
```
3. Navigate the the repository folder
```sh
$ cd parco-homework/
```
4. Edit the `IPC_homework.pbs` file to set the correct parameters for your job, in the `# --- COMPILING AND RUNNING ---` section
```sh
# select the working directory
cd <your/path>/parco-homework-D2/src/>

[...]

# change to executables directory
cd <your/path>/parco-homework-D2/bin/
```

> :warning: **Selecting the number of processors**:\
The `IPC_homework.pbs` file is already set to run the code with different number of processors. You can change the number of processors by modifying the `for` loop in the `# --- COMPILING AND RUNNING ---` section.\
If you want to run the code with a fixed number of processors, you can remove the `for` loop and set the number of processors in the `mpiexec -np $num_procs ./homework_exe` command.
```sh 
# run the code with different number of processors
for num_procs in 1 2 4 8 16 32 64 96; do
    export OMP_NUM_THREADS=$num_procs; # set number of OMP threads equal to the number of processors
    if ! mpiexec -np $num_procs ./homework_exe; then
        echo "Execution failed for $num_procs processors"
        exit 1
    fi
done

# run the code with a fixed number of processors
export OMP_NUM_THREADS=<n_cpus>; # set number of OMP threads
mpiexec -np <n_cpus> ./homework_exe
```

5. Submit PBS job to the queue
```sh
$ qsub IPC_homework.pbs
```

## On your local machine

### Prerequisites
- Multicore CPU (to effectively run MPI tests)

### Steps
1. Clone the repository
```sh
$ git clone https://github.com/chiarasabaini/parco-homework-D2.git
```
2. Navigate the the repository folder
```sh
$ cd parco-homework/
```
3. Create a build directory
```sh
$ mkdir build
```
4. Navigate to the build directory
```sh
$ cd build/
```
5. Run the following commands:
```sh
$ cmake ..
$ make
```
6. Run the executable (generated in the `bin/` folder)
```sh
$ mpiexec -np <n_cpus> ../bin/project # n_cpus : number of CPUs to use
```

## Data Analysis

You will find all of the .csv files containing the data inside the `data/` folder.
You can process the data and plot the graphs using the provided Jupyter Notebook in the `src/` folder.

Happy testing!
---

