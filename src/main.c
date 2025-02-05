#include "test.h"
#include "utils.h"
#include "matrix_operations.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    seq_log = init_log(SEQUENTIAL);
    mpi_log = init_log(MPI);
    omp_log = init_log(OMP);
    
    for(int i=0; i < 5; i++){
        test_performance(&argc, &argv);
    }

    close_log(seq_log);
    close_log(mpi_log);
    close_log(omp_log);

    return 0;
}
