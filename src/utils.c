#include "utils.h"

#include <mpi.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const char* func2str(func_t function) {
    switch (function) {
        case TRANSPOSITION:
            return "TRANSPOSITION";
        case SYMMETRY:
            return "SYMMETRY";
        default:
            return "UNKNOWN";
    }
}

const char* imp2str(impl_t implementation) {
    switch (implementation) {
        case SEQUENTIAL:
            return "SEQUENTIAL";
        case OMP:
            return "OMP";
        case MPI:
            return "MPI";
        default:
            return "UNKNOWN";
    }
}

const char* mpi2str(mpi_t mpi_type) {
    switch (mpi_type) {
        case SCATTER:
            return "SCATTER";
        case BROADCAST:
            return "BROADCAST";
        case REDUCE:
            return "REDUCE";
        default:
            return "UNKNOWN";
    }
}

// LOG

FILE* seq_log;
FILE* mpi_log;
FILE* omp_log;

FILE* init_log(impl_t impl) {
    time_t current_time;
    time(&current_time);
    
    struct tm* time_info;
    time_info = localtime(&current_time);

    char time_string[32];
    strftime(time_string, 32, "%Y%m%d_%H%M%S", time_info);

    // int n_threads = get_num_threads();
    int n_cpus;
    MPI_Comm_size(MPI_COMM_WORLD, &n_cpus);

    char filepath[255];
    snprintf(filepath, 255, "../out/data/%s_%s_%d_log.csv", time_string, imp2str(impl), n_cpus);

    FILE* log = fopen(filepath, "w");
    if (log == NULL) {
        perror("Error opening file");
    } else {
        switch(impl){
        case SEQUENTIAL:
            fprintf(log, "Matrix Size,Function,Implementation,Execution Time\n");
            break;
        case MPI:
            fprintf(log, "Matrix Size,CPUs,Function,Implementation,MPI Implementation,Execution Time (total), Execution Time (no msg)\n");
            break;
        case OMP:
            fprintf(log, "Matrix Size,Threads,Function,Implementation,Execution Time\n");
            break;       
        }
    }
        
    return log;
}


void print_log_seq(FILE* log, const char* msg, func_t func, impl_t imp, int size, double execution_time) {

    #if LOG_DEBUG == 1
        printf("%s:\n\tmatrix size: %d\nexecution time:%f\n", msg, size, execution_time);
    #endif

    fprintf(log, "%d,%s,%s,%0.9f\n", size, func2str(func), imp2str(imp), execution_time);
}


void print_log_omp(FILE* log, const char* msg, func_t func, impl_t imp, int size, int n_threads, double execution_time) {

    #if LOG_DEBUG == 1
        printf("%s:\n\tmatrix size: %d\n\tn_threads: %d\n\texecution time:%f\n", msg, size, n_threads, execution_time);
    #endif

    fprintf(log, "%d,%d,%s,%s,%0.9f\n", size, n_threads, func2str(func), imp2str(imp), execution_time);
}


void print_log_mpi(FILE* log, const char* msg, func_t func, impl_t imp, mpi_t mpi_type, int size, int n_cpus, double execution_time_tot, double execution_time_no_msg) {

    #if LOG_DEBUG == 1
        printf("%s:\n\tmatrix size: %d\n\tn_cpus: %d\n\texecution time tot:%f\n\texecution time no msg:%f\n", msg, size, n_cpus, execution_time_tot, execution_time_no_msg);
    #endif

    fprintf(log, "%d,%d,%s,%s,%s,%0.9f\n", size, n_cpus, func2str(func), imp2str(imp), mpi2str(mpi_type), execution_time_tot, execution_time_no_msg);
}


void close_log(FILE* log) {
    if(log) {
        fclose(log);
    }
}


void print_matrix(float* M, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%f | ", M[i * size + j]);
        }
        printf("\n");
    }
    printf("\n");
}


// MATRIX
float* new_mat(int rows, int cols) {
    float* M = malloc(sizeof(float) * rows * cols);

    return M;
}


void init_mat(float* M, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i * n + j] = fabs((float)rand()) / RAND_MAX; // Correct indexing
        }
    }
}


void init_symmetric_mat(float* M, int n) {
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            M[i * n + j] = ((float)rand()) / RAND_MAX;

            if (i != j) { // off-diagonal elements
                M[j * n + i] = M[i * n + j];
            }
        }
    }
}


// function to free dynamically allocated memory for matrix
void free_mat(float* M, int rows) {
    free(M);
}

int get_num_threads() {
    const char *env_threads = getenv("OMP_NUM_THREADS");
    if(env_threads) {
        return atoi(env_threads);
    }  else {
        return 1;
    }
}