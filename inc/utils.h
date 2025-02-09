/**
 * @file utils.h
 * @brief Header file for useful functions
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>

#define MIN_MAT_SIZE 16
#define MAX_MAT_SIZE 4096
// #define LOG_DEBUG 1

/**
 * @brief Functions that can be tested
 */
typedef enum {
    TRANSPOSITION = 0,
    SYMMETRY = 1,
    N_FUNCTIONS
} func_t;

/**
 * @brief Convert func_t to string
 * 
 * @param function function type
 * @return const char* 
 */
const char* func2str(func_t function);

/**
 * @brief Implementations that can be tested
 */


typedef enum {
  SEQUENTIAL = 0,
  OMP = 1,
  MPI = 2,
  N_IMPLEMENTATIONS
} impl_t;

/**
 * @brief types of MPI that can be tested
 */
typedef enum {
    SCATTER = 0,
    BROADCAST = 1,
    REDUCE = 2,
    N_MPI_IMPLEMENTATIONS
} mpi_t;

/**
 * @brief Convert imp_t to string
 * 
 * @param implementation implementation type
 * @return const char* 
 */
const char* mpi2str(mpi_t mpi_type);

// MATRIX

/**
 * @brief Initialize a matrix with random values
 * 
 * @param M matrix
 * @param n size of matrix M[n][n]
 */
float* new_mat(int rows, int cols);


/**
 * @brief Free previously allocated matrix memory
 * 
 * @param M matrix
 * @param n size of matrix M[n][n]
 */
void free_mat(float* M, int rows);


/**
 * @brief Initialize given matrix, populating it with random values
 * 
 * @param M matrix
 * @param n size of matrix M[n][n]
 */
void init_mat(float* M, int n);


/**
 * @brief Initialize given matrix, populating it with random values to make it symmetric
 * 
 * @param M matrix
 * @param n size of matrix M[n][n]
 */
void init_symmetric_mat(float* M, int n);


// LOG

extern FILE* seq_log;
extern FILE* mpi_log;
extern FILE* omp_log;

/**
 * @brief Open and initialize log file
 * 
 * @param impl implementation whose data are being logged
 * 
 * @return FILE*
 */
FILE* init_log(impl_t impl);


/**
 * @brief Close previously opened log file
 * 
 * @param log file pointer
 */
void close_log(FILE *log);


/**
 * @brief Print log string on file and, if in debugging mode, on screen
 * 
 * @param log log file
 * @param msg debug message
 * @param func executing function
 * @param imp implementation type
 * @param size matrix size
 * @param execution_time time elapsed between start and end of execution of the function
 */
void print_log_seq(FILE* log, const char* msg, func_t func, impl_t imp, int size, double execution_time);


/**
 * @brief Print log string on file and, if in debugging mode, on screen
 * 
 * @param log log file
 * @param msg debug message
 * @param func executing function 
 * @param imp implementation type
 * @param size matrix size
 * @param n_threads number of threads used to run
 * @param execution_time time elapsed between start and end of execution of the function
 */
void print_log_omp(FILE *log, const char *msg, func_t func, impl_t imp, int size, int n_threads, double execution_time);


/**
 * @brief Print log string on file and, if in debugging mode, on screen
 * 
 * @param log log file
 * @param msg debug message
 * @param func executing function
 * @param imp implementation type
 * @param size matrix size
 * @param n_cpus number of cpus used to run
 * @param execution_time_tot time elapsed between start and end of the function
 * @param execution_time_no_msg time elapsed between start and end of the function, not counting the time needed to pass messages
 */
void print_log_mpi(FILE* log, const char* msg, func_t func, impl_t imp, mpi_t mpi_type, int size, int n_cpus, double execution_time_tot, double execution_time_no_msg);

#endif // UTILS_H