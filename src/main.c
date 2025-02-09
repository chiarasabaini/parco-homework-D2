#include "test.h"
#include "utils.h"
#include "matrix_operations.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    MPI_Init(&argc, &argv);

    seq_log = init_log(SEQUENTIAL);
    mpi_log = init_log(MPI);
    omp_log = init_log(OMP);

    

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // for(int i=0; i < 5; i++){
    //     test_performance(rank, size);
    // }

    omp_set_num_threads(4);

    test_performance(rank, size);
    
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

    close_log(seq_log);
    close_log(mpi_log);
    close_log(omp_log);

    return 0;
}
