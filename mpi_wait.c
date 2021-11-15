#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    int process_Rank, size_Of_Cluster, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    for(i = 0, i < size_Of_Cluster, i++){
        if(i == process_Rank){
            printf("Hello World from process %d of %d at flag 1\n", process_Rank, size_Of_Cluster);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }    

    MPI_Barrier(MPI_COMM_WORLD);
    if (process_Rank == 0){
        printf("All processes passed flag 1");
    }
    
    for(i = 0, i < size_Of_Cluster, i++){
        if(i == process_Rank){
            printf("Hello World from process %d of %d at flag 2\n", process_Rank, size_Of_Cluster);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }    

    MPI_Barrier(MPI_COMM_WORLD);
    if (process_Rank == 0){
        printf("All processes passed flag 2");
    }

    MPI_Finalize();
    return 0;
}
