#include "test.h"
#include "utils.h"
#include "matrix_operations.h"

/**
 * @brief
 * 
 * @param M matrix
 * @param T matrix
 */    
void test_performance(int argc, char* argv[]){
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for(int mat_size = MIN_MAT_SIZE; mat_size <= MAX_MAT_SIZE; mat_size *= 2){
        // allocate matrices, only on rank 0
        float** M = (rank == 0) ? new_mat(mat_size) : NULL;
        float** T = (rank == 0) ? new_mat(mat_size) : NULL;

        if (rank==0){
            // test symmetry check
            for (int i = 0; i < 5; i++) {
                init_symmetric_mat(M, mat_size);

                // TASK 1: sequential code
                checkSym(M, mat_size);
                matTranspose(M, T, mat_size);
                check_transpose(M, T, mat_size);
                // print_matrix(T, size);

                // TASK 2: parallelization using MPI
                checkSymMPI(M, mat_size, rank, size, MPI_COMM_WORLD);
                matTransposeMPI(M, T, mat_size, rank, size, MPI_COMM_WORLD);
                check_transpose(M, T, mat_size);
                // print_matrix(T, size);

                
                // TASK 4: parallelization using OMP
                checkSymOMP(M, mat_size);
                matTransposeOMP(M, T, mat_size);
                check_transpose(M, T, mat_size);

            }

            for (int i = 0; i < 5; i++) {
                init_mat(M, mat_size);
                
                // TASK 1: sequential matrix transposition
                matTranspose(M, T, mat_size);
                check_transpose(M, T, mat_size);
                // print_matrix(T, size);

                // TASK 2: parallelization using MPI
                matTransposeMPI(M, T, mat_size, rank, size, MPI_COMM_WORLD);
                check_transpose(M, T, mat_size);
                // print_matrix(T, size);

                // TASK 4: parallelization using OMP
                matTransposeMPI(M, T, mat_size);
                check_transpose(M, T, mat_size);
                // print_matrix(T, size);
            }
        }

        // free matrices memory
        if (M != NULL) {
            free_mat(M, size);
        }
        if (T != NULL) {
            free_mat(T, size);
        }
    }

    MPI_Finalize();
}
