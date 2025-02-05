/**
 * @file matrix_operations.h
 * @brief Header file for matrix operations
 */

#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <mpi.h>
#include <stdbool.h>


// TASK 1
/**
 * @brief Check if a matrix is symmetric
 * 
 * @param M matrix
 * @param n size of matrix M[n][n]
 * @return true if the matrix is symmetric, false otherwise
 */
bool checkSym(float** M, int n);


/**
 * @brief Transpose a given matrix
 * 
 * @param[in] M matrix
 * @param[in] n size of matrix M[n][n]
 * @param[out] T result of the transposition
 */
void matTranspose(float** M, float** T, int n);


// TASK 2
/**
 * @brief Check if a matrix is symmetric, parallelized using MPI
 * 
 * @param M matrix
 * @param n size of matrix M[n][n]
 * @return true if the matrix is symmetric, false otherwise
 */
bool checkSymMPI(float** M, int n, int rank, int n_cpus, MPI_Comm comm);


/**
 * @brief Transpose a given matrix, parallelized using MPI
 * 
 * @param[in] M matrix
 * @param[in] n size of matrix M[n][n]
 * @param[out] T result of the transposition
 */
void matTransposeMPI(float** M, float** T, int n, int rank, int n_cpus, MPI_Comm comm);



// TASK 4
/**
 * @brief Check if a matrix is symmetric, parallelized using OMP
 * 
 * @param M matrix
 * @param n size of matrix M[n][n]
 * @return true if the matrix is symmetric, false otherwise
 */
bool checkSymOMP(float** M, int n);


/**
 * @brief Transpose a given matrix, parallelized using OMP
 * 
 * @param[in] M matrix
 * @param[in] n size of matrix M[n][n]
 * @param[out] T result of the transposition
 */
void matTransposeOMP(float** M, float** T, int n);


// TEST
/**
 * @brief Check if transposition return the correct result
 * 
 * @param M matrix
 * @param T result of transposition
 * @param size size of given matrices
 * @return true if it's been trasposed correctly, false otherwise
 */
bool check_transpose(float **M, float **T, int size);


#endif // MATRIX_OPERATIONS_H