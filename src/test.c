#include "test.h"
#include "utils.h"
#include "matrix_operations.h"

/**
 * @brief
 * 
 * @param M matrix
 * @param T matrix
 */    
void test_performance(int rank, int size){

    //for(int mat_size = MIN_MAT_SIZE; mat_size <= MAX_MAT_SIZE; mat_size *= 2){
        // allocate matrices
        int mat_size = 4;

        if (rank==0){

            float** M = new_mat(mat_size, mat_size);
            float** T = new_mat(mat_size, mat_size);

            // test symmetry check
            for (int i = 0; i < 5; i++) {
                init_symmetric_mat(M, mat_size);

                // TASK 1: sequential code
                checkSym(M, mat_size);
                matTranspose(M, T, mat_size);
                check_transpose(M, T, mat_size);
                // print_matrix(T, size);

                // TASK 2: parallelization using MPI
                checkSymMPI(M, mat_size, rank, size);
                matTransposeMPI(M, T, mat_size, rank, size);
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
                matTransposeMPI(M, T, mat_size, rank, size);
                check_transpose(M, T, mat_size);
                // print_matrix(T, size);

                // TASK 4: parallelization using OMP
                matTransposeOMP(M, T, mat_size);
                check_transpose(M, T, mat_size);
                // print_matrix(T, size);
            }
            // free matrices memory
            if (M != NULL) {
                free_mat(M, mat_size);
            }
            if (T != NULL) {
                free_mat(T, mat_size);
            }
        }
    // }


}
