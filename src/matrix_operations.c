#include "utils.h"
#include "matrix_operations.h"

#include <mpi.h>
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// TASK 1
bool checkSym(float** M, int n) {
    double start = omp_get_wtime();
    bool isSym = true;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (M[i][j] != M[j][i]) {
                isSym = false;
                break;
            }
        }
        if (!isSym){
            break;
        }
    }

    double end = omp_get_wtime();
    int n_threads = atoi(getenv("OMP_NUM_THREADS"));
    print_log_omp(global_log, "Sequential Symmetry Check", SYMMETRY, SEQUENTIAL, n, n_threads, end - start);

    return isSym;
}


void matTranspose(float** M, float** T, int n) {
    double start = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[j][i] = M[i][j];
        }
    }

    double end = omp_get_wtime();
    int n_threads = atoi(getenv("OMP_NUM_THREADS"));
    print_log_omp(global_log, "Sequental Transposition", TRANSPOSITION, SEQUENTIAL, n, n_threads, end - start);
}


// TASK 2
bool checkSymMPI(float** M, int n) {
    int rank, n_cpus;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_cpus);

    int chunk_size = (n - 1) / n_cpus;
    int start_row = rank * chunk_size;
    int end_row = (rank == n_cpus - 1) ? (n - 1) : (start_row + chunk_size);

    bool localSym = true;

    for (int i = start_row; i <= end_row; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (M[i][j] != M[j][i]) {
                localSym = false;
                break;
            }
        }
        if (!localSym) {
            break;
        }
    }

    bool isSym;
    MPI_Allreduce(&localSym, &isSym, 1, MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD); // Ensure all processes have finished before timing ends

    double end = MPI_Wtime();
    print_log_omp(global_log, "MPI Parallelized Symmetry Check", SYMMETRY, MPI_OP, n, n_cpus, end - start);

    return isSym;
}


void matTransposeMPI(float** M, float** T, int n) {
    int rank, n_cpus;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_cpus);

    int chunk_size = n / n_cpus; 
    int start_row = rank * chunk_size;
    int end_row = (rank == n_cpus - 1) ? n : (start_row + chunk_size);

    // Allocate memory for the local chunk of the transpose
    float** local_T = (float**)malloc(chunk_size * sizeof(float*));
    for (int i = 0; i < chunk_size; ++i) {
        local_T[i] = (float*)malloc(n * sizeof(float));
    }

    // Transpose the local chunk
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < n; ++j) {
            local_T[i - start_row][j] = M[i][j]; 
        }
    }

    // Gather all transposed chunks to rank 0
    if (rank == 0) {
        for (int i = 0; i < n_cpus; ++i) {
            int current_start_row = i * chunk_size;
            int current_end_row = (i == n_cpus - 1) ? n : (current_start_row + chunk_size);

            for (int row_in_chunk = 0; row_in_chunk < current_end_row - current_start_row; ++row_in_chunk) {
                for (int col = 0; col < n; ++col) {
                    if (i == 0) { 
                        // Rank 0 already has its own chunk
                        continue;
                    } else {
                        MPI_Recv(&T[col][current_start_row + row_in_chunk], 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
                    }
                }
            }
        }
    } else { 
        for (int row_in_chunk = 0; row_in_chunk < chunk_size; ++row_in_chunk) {
            for (int col = 0; col < n; ++col) {
                MPI_Send(&local_T[row_in_chunk][col], 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            }
        }
    }

    // Free memory for the local chunk
    for (int i = 0; i < chunk_size; ++i) {
        free(local_T[i]);
    }
    free(local_T);

    MPI_Barrier(MPI_COMM_WORLD); // Ensure all processes have finished before timing ends

    double end = MPI_Wtime();
    print_log_omp(global_log, "MPI Parallelized Transposition", TRANSPOSITION, MPI_OP, n, n_cpus, end - start);
}


// TASK 4
bool checkSymOMP(float** M, int n) {
    double start = omp_get_wtime();

    int i;
    bool isSym = true;

    #pragma omp parallel for collapse(1) reduction(&:is_sym)
    for (i = 0; i < n - 1; i++) {

        #pragma omp parallel for collapse(1) reduction(&:is_sym)
        for (int j = i + 1; j < n; j++) {
            if (M[i][j] != M[j][i]) {
                isSym = false;
        }
    }
    }

    double end = omp_get_wtime();
    int n_threads = atoi(getenv("OMP_NUM_THREADS"));
    print_log_omp(global_log, "OMP Parallelized Symmetry Check", SYMMETRY, OMP, n, n_threads, end - start);

    return isSym;
}


void matTransposeOMP(float** M, float** T, int n){
    double start = omp_get_wtime();

    int i, j;
    
    #pragma omp parallel for collapse(2)
    for (i=0; i < n; i++) {
        for (j=0; j < n; j++) {
            T[j][i] = M[i][j];
        }
    }


    double end = omp_get_wtime();
    int n_threads = atoi(getenv("OMP_NUM_THREADS"));
    print_log_omp(global_log, "OMP Parallelized Transposition", TRANSPOSITION, OMP, n, n_threads, end - start);
}


// TEST
bool check_transpose(float** M, float** T, int size){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (M[i][j] != T[j][i]) {
                printf("TRANSPOSITION WENT WRONG!");
                return false;
            }
        }
    }
    return true;
}
