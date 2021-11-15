#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define RC 3                   /* row and col size */
#define MAIN 0                 /* taskid of first task */
#define FROM_MAIN 1            /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int main (int argc, char *argv[])
{
int	numtasks,              /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                 /* message type */
	rows,                  /* rows of matrix A sent to each worker */
	averow, extra, offset, /* used to determine rows sent to each worker */
	i, j, k, rc;           
double	a[RC][RC],           /* matrix A */
	    b[RC][RC],           /* matrix B */
	    c[RC][RC];           /* result matrix C */

MPI_Status status;


MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
numworkers = numtasks-1;


/**************************** main task ************************************/
   if (taskid == MAIN)
   {
      printf("mpi_mm has started with %d tasks.\n",numtasks);
      for (i=0; i<RC; i++)
         for (j=0; j<RC; j++)
            a[i][j]= i;
      for (i=0; i<RC; i++)
         for (j=0; j<RC; j++)
            b[i][j]= j;

      /* Measure start time */
      double start = MPI_Wtime();

      averow = RC/numworkers;
      extra = RC%numworkers;
      offset = 0;
      mtype = FROM_MAIN;
      for (dest=1; dest<=numworkers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&a[offset][0], rows*RC, MPI_DOUBLE, dest, mtype,
                   MPI_COMM_WORLD);
         MPI_Send(&b, RC*RC, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         offset = offset + rows;
      }

      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&c[offset][0], rows*RC, MPI_DOUBLE, source, mtype, 
                  MPI_COMM_WORLD, &status);
      }

      double finish = MPI_Wtime();
      printf("Done in %f seconds.\n", finish - start);
      printf("mpi_mm has started with %d tasks.\n",numtasks);
      printf("a = \n");
      for (i=0; i<RC; i++){
         for (j=0; j<RC; j++){
            printf("%1.3f ", a[i][j]);
         }
         printf("\n");
      }
      printf("b = \n");
      for (i=0; i<RC; i++){
         for (j=0; j<RC; j++){
            printf("%1.3f ", b[i][j]);
         }
         printf("\n");
      }
      printf("c = \n");
      for (i=0; i<RC; i++){
         for (j=0; j<RC; j++){
            printf("%1.3f ", c[i][j]);
         }
         printf("\n");
      }
   }

/**************************** worker task ************************************/
   if (taskid > MAIN)
   {
      mtype = FROM_MAIN;
      MPI_Recv(&offset, 1, MPI_INT, MAIN, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MAIN, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&a, rows*RC, MPI_DOUBLE, MAIN, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, RC*RC, MPI_DOUBLE, MAIN, mtype, MPI_COMM_WORLD, &status);

      for (k=0; k<RC; k++)
         for (i=0; i<rows; i++)
         {
            c[i][k] = 0.0;
            for (j=0; j<RC; j++)
               c[i][k] = c[i][k] + a[i][j] * b[j][k];
         }
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MAIN, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MAIN, mtype, MPI_COMM_WORLD);
      MPI_Send(&c, rows*RC, MPI_DOUBLE, MAIN, mtype, MPI_COMM_WORLD);
   }
   MPI_Finalize();
}
