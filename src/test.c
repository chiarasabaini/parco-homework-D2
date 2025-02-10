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
    int min_mat_size = get_min_mat_size();

    for(int mat_size = min_mat_size; mat_size <= MAX_MAT_SIZE; mat_size *= 2){
        float* M = NULL;
        float* T = NULL;
        if (rank==0){
            M = new_mat(mat_size, mat_size);
            T = new_mat(mat_size, mat_size);
        }
        
        // test symmetry check
        for (int i = 0; i < 5; i++) {
            if(rank == 0) {
                init_symmetric_mat(M, mat_size);
            }

            // TASK 1: sequential code
            if(rank == 0) {
                checkSym(M, mat_size);
                matTranspose(M, T, mat_size);
                check_transpose(M, T, mat_size);
            }
            // print_matrix(T, size);

            // TASK 2: parallelization using MPI
            
            checkSymMPI(M, mat_size, rank, size);
            matTransposeMPI(M, T, mat_size, rank, size);
            if(rank == 0) {
                check_transpose(M, T, mat_size);
            }
            
            // TASK 4: parallelization using OMP
            if(rank == 0) {
                checkSymOMP(M, mat_size);
                matTransposeOMP(M, T, mat_size);
                check_transpose(M, T, mat_size);
            }

        }
        // if(rank == 0) {
        //     if (M != NULL) {
        //         free_mat(M, mat_size);
        //     }
        //     if (T != NULL) {
        //         free_mat(T, mat_size);
        //     }
        // }

        for (int i = 0; i < 5; i++) {
            if(rank == 0) {
                init_mat(M, mat_size);
            }
            
            // TASK 1: sequential matrix transposition
            if(rank == 0) {
                matTranspose(M, T, mat_size);
                check_transpose(M, T, mat_size);
            }

            // TASK 2: parallelization using MPI
            matTransposeMPI(M, T, mat_size, rank, size);
            if(rank == 0) {
                check_transpose(M, T, mat_size);
            }

            matTransposeMPI_Bcast(M, T, mat_size, rank, size);
            if(rank == 0) {
                check_transpose(M, T, mat_size);
            }
            // print_matrix(T, size);

            // TASK 4: parallelization using OMP
            if(rank == 0) {
                matTransposeOMP(M, T, mat_size);
                check_transpose(M, T, mat_size);
            }
            // print_matrix(T, size);
        }
        // free matrices memory
        if(rank == 0) {
            if (M != NULL) {
                free_mat(M, mat_size);
            }
            if (T != NULL) {
                free_mat(T, mat_size);
            }
        }
    }


}
