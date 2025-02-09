#include "utils.h"
#include "matrix_operations.h"

#include <mpi.h>
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// TASK 1
bool checkSym(float* M, int n) {
    double start = omp_get_wtime();
    bool isSym = true;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (M[i * n + j] != M[j * n + i]) { // Correct indexing for contiguous array
                isSym = false;
                break;
            }
        }
        if (!isSym) {
            break;
        }
    }

    double end = omp_get_wtime();
    print_log_seq(seq_log, "Sequential Symmetry Check", SYMMETRY, SEQUENTIAL, n, end - start);

    return isSym;
}


void matTranspose(float* M, float* T, int n) {
    double start = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[j * n + i] = M[i * n + j]; 
        }
    }

    double end = omp_get_wtime();
    print_log_seq(seq_log, "Sequential Transposition", TRANSPOSITION, SEQUENTIAL, n, end - start);
}


// TASK 2
bool checkSymMPI(float* M, int n, int rank, int n_cpus) {
    double start_total = MPI_Wtime();

    int chunk_size = (n - 1) / n_cpus;
    int start_row = rank * chunk_size;
    int end_row = (rank == n_cpus - 1) ? (n - 1) : (start_row + chunk_size);

    bool localSym = true;

    double start_compute = MPI_Wtime();

    for (int i = start_row; i <= end_row; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (M[i * n + j] != M[j * n + i]) {  // Correct indexing for contiguous array
                localSym = false;
                break;
            }
        }
        if (!localSym) {
            break;
        }
    }

    double end_compute = MPI_Wtime();

    bool isSym = false;
    MPI_Allreduce(&localSym, &isSym, 1, MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD); // Ensure all processes have finished before timing ends

    double end_total = MPI_Wtime();
    print_log_mpi(mpi_log, "MPI Parallelized Symmetry Check", SYMMETRY, MPI, n, n_cpus, end_total - start_total, end_compute - start_compute);

    return isSym;
}

// MPI Scatter - Gather with Datatypes
void matTransposeMPI(float* M, float* T, int mat_size, int rank, int n_cpus) {
    double start_total = MPI_Wtime();

    int chunk_size = mat_size / n_cpus; // elements per chunk (columns per cpu)

    // create datatype to scatter
    MPI_Datatype cols_type, resized_cols_type;
    MPI_Type_vector(mat_size, // n blocks of contiguous elements (one for each row)
                    chunk_size, // elements in each block (columns per cpu)
                    mat_size, // offset between start of block N and start of block N+1 (matrix width)
                    MPI_FLOAT, &cols_type);
    MPI_Type_create_resized(cols_type, 0, chunk_size * sizeof(float), &resized_cols_type); // bytes for each block (elements in block * size of each element)

    MPI_Type_commit(&resized_cols_type);

    int counts[n_cpus], offset[n_cpus];
    if (rank == 0) {
        for (int i = 0; i < n_cpus; i++) {
            counts[i] = 1;
            offset[i] = i;
        }
    }

    // scattering columns to all cpus
    float* local_M = new_mat(chunk_size, mat_size);
    MPI_Scatterv(M, counts, offset, resized_cols_type, local_M, chunk_size * mat_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    //printf("Process %d received in local_matrix after SCATTERV:\n", rank);
    //print_matrix(local_M, N, chunk_size);

    // local chunk transpose
    float* local_T = new_mat(mat_size, chunk_size);
    double start_compute = MPI_Wtime();
    for (int i = 0; i < mat_size; i++) {
        for (int j = 0; j < chunk_size; j++) {
            local_T[j * mat_size + i] = local_M[i * cols + j];
        }
    }
    double end_compute = MPI_Wtime();
    //printf("Process %d local_T after transpose:\n", rank);
    //print_matrix(local_T, chunk_size, N);

    // gather transposed chunk
    MPI_Gather(local_T, mat_size * chunk_size, MPI_FLOAT, T, mat_size * chunk_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    free_mat(local_M, chunk_size);
    free_mat(local_T, mat_size);
    MPI_Type_free(&resized_cols_type);

    double end_total = MPI_Wtime();
    print_log_mpi(mpi_log, "MPI Parallelized Transposition", TRANSPOSITION, MPI, mat_size, n_cpus, end_total - start_total, end_compute - start_compute);
}


// MPI Broadcast
void matTransposeMPI_Bcast(float* M, float* T, int mat_size, int rank, int n_cpus){
    double start_total = MPI_Wtime();

    int chunk_size = mat_size / n_cpus;
    int start = rank * chunk_size;
    int end   = start + chunk_size;

    // broadcast M to all cpus
    if (rank != 0) {
        // allocate space for M
        M = new_mat(mat_size, mat_size);
    }
    MPI_Bcast(M, mat_size * mat_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // local chunk transpose
    double start_compute = MPI_Wtime();

    float* local_T = new_mat(chunk_size, mat_size);
    for (int i = start; i < end; i++) {
        for (int j = 0; j < mat_size; j++) {
            local_T[(i - start) * mat_size + j] = M[j * mat_size + i];
        }
    }

    double end_compute = MPI_Wtime();

    // gather transposed chunk
    MPI_Gather(local_T,  chunk_size * mat_size, MPI_FLOAT, T, chunk_size * mat_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    free_mat(local_T, mat_size);
    if (rank != 0) {
        free_mat(M, mat_size);
    }

    double end_total = MPI_Wtime();
    print_log_mpi(mpi_log, "MPI Parallelized Transposition", TRANSPOSITION, MPI, mat_size, n_cpus, end_total - start_total, end_compute - start_compute);
}


// TASK 4
bool checkSymOMP(float* M, int n) {
    double start = omp_get_wtime();

    int i;
    bool isSym = true;

    #pragma omp parallel for collapse(1) reduction(&:isSym)
    for (i = 0; i < n - 1; i++) {

        #pragma omp parallel for collapse(1) reduction(&:isSym)
        for (int j = i + 1; j < n; j++) {
            if (M[i * n + j] != M[j * n + i]) {
                isSym = false;
            }
        }
    }

    double end = omp_get_wtime();
    int n_threads = atoi(getenv("OMP_NUM_THREADS"));
    print_log_omp(omp_log, "OMP Parallelized Symmetry Check", SYMMETRY, OMP, n, n_threads, end - start);

    return isSym;
}


void matTransposeOMP(float* M, float* T, int n){
    double start = omp_get_wtime();

    int i, j;
    
    #pragma omp parallel for collapse(2)
    for (i=0; i < n; i++) {
        for (j=0; j < n; j++) {
            T[j * n + i] = M[i * n + j];
        }
    }


    double end = omp_get_wtime();
    int n_threads = atoi(getenv("OMP_NUM_THREADS"));
    print_log_omp(omp_log, "OMP Parallelized Transposition", TRANSPOSITION, OMP, n, n_threads, end - start);
}


// TEST
bool check_transpose(float* M, float* T, int size){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (M[i * size + j] != T[j * size + i]) {
                printf("TRANSPOSITION WENT WRONG!\n");
                return false;
            }
        }
    }
    return true;
}
