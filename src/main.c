#include "test.h"
#include "utils.h"
#include "matrix_operations.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank=0=){
        seq_log = init_log(SEQUENTIAL);
        mpi_log = init_log(MPI);
        omp_log = init_log(OMP);
    }
        
    
    for(int i=0; i < 5; i++){
        test_performance();
    }

    if (rank=0=){
        close_log(seq_log);
        close_log(mpi_log);
        close_log(omp_log);
    }

    MPI_Finalize();

    return 0;
}
