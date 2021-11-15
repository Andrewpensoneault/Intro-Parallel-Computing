#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char* argv[])
{
    int root_rank = 0;
    int my_rank;
    MPI_Init(&argc, &argv);
 
    // Get number of processes and check that 4 processes are used
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size != 4)
    {
        printf("This application is meant to be run with 4 MPI processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int my_value = my_rank * 100;
    printf("Process %d, my value = %d.\n", my_rank, my_value);
 
    if(my_rank == root_rank)
    {
        int buffer[4];
        MPI_Gather(&my_value, 1, MPI_INT, buffer, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
        printf("Values collected on process %d: %d, %d, %d, %d.\n", my_rank, buffer[0], buffer[1], buffer[2], buffer[3]);
    }
    else
    {
        MPI_Gather(&my_value, 1, MPI_INT, NULL, 0, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}