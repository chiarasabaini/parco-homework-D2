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
    print_log_seq(seq_log, "Sequential Symmetry Check", SYMMETRY, SEQUENTIAL, n, end - start);

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
    print_log_seq(seq_log, "Sequental Transposition", TRANSPOSITION, SEQUENTIAL, n, end - start);
}


// TASK 2
bool checkSymMPI(float** M, int n, int rank, int n_cpus, MPI_Comm comm) {
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

    double end_after_msg = MPI_Wtime();
    print_log_mpi(mpi_log, "MPI Parallelized Symmetry Check", SYMMETRY, MPI_OP, n, n_cpus, end_before_msg - start_before_msg, end_after_msg - start_after_msg);

    return isSym;
}


void matTransposeMPI(float** M, float** T, int n, int rank, int n_cpus, MPI_Comm comm) {

    // chunk size for each process
    int chunk_size = n / n_cpus 
    int extra = n % n_cpus; 
    int offset = rank * chunk_size + (rank < extra ? rank : extra);

    // allocate memory for the local chunk of the transpose
    float* local_M = (float*)malloc(block_size * n * sizeof(float));
    float* local_T = (float*)malloc(block_size * n * sizeof(float));

    double start_total = MPI_Wtime();

    // scatter rows to each process
    MPI_Scatter(M[offset], block_size * n, MPI_FLOAT, local_M, block_size * n, MPI_FLOAT, 0, comm);

    double start_compute = MPI_Wtime(); 

    // Transpose the local chunk
    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < n; j++) {
            local_T[j * chunk_size + i] = local_M[i * n + j]; 
        }
    }

    double end_compute = MPI_Wtime();

    // gather transposed chunks to root process
    MPI_Gather(local_T, chunk_size * n, MPI_FLOAT, 
               T[offset], chunk_size * n, MPI_FLOAT, 0, comm);

    double end_total = MPI_Wtime(); 

    double compute_time = end_compute - start_compute;
    double total_time = end_total - start_total;

    free(local_M);
    free(local_T);

    print_log_mpi(mpi_log, "MPI Parallelized Transposition", TRANSPOSITION, MPI_OP, n, n_cpus, compute_time, total_time);
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
    print_log_omp(omp_log, "OMP Parallelized Symmetry Check", SYMMETRY, OMP, n, n_threads, end - start);

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
    print_log_omp(omp_log, "OMP Parallelized Transposition", TRANSPOSITION, OMP, n, n_threads, end - start);
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
