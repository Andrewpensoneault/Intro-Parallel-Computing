#include <stdio.h>
#include <cuda_runtime.h>
#include <stdlib.h>

//Device code
__global__
void add(int *a, int *b, int *c) {
int index = threadIdx.x + blockIdx.x * blockDim.x;
c[index] = a[index] + b[index];
}

//Host code
int main(int argc, char* argv[]) {

int i, size, tot_n, tot_size, blocknum, threadnum;
int *a, *b, *c; // host copies of a, b, c
int *d_a, *d_b, *d_c; // device copies of a, b, c

size = sizeof(int);
blocknum = atoi(argv[1]);
threadnum = atoi(argv[2]);
tot_n = blocknum*threadnum;
tot_size = size*tot_n;

a = (int *) malloc(tot_size);
b = (int *) malloc(tot_size);
c = (int *) malloc(tot_size);

for(i=0; i<tot_n; i++){
a[i] = i;
b[i] = tot_n-i;
c[i] = 0;
};


// Allocate space for device copies of a, b, c
cudaMalloc(&d_a, tot_size);
cudaMalloc(&d_b, tot_size);
cudaMalloc(&d_c, tot_size);

// Copy inputs to device
cudaMemcpy(d_a, a, tot_size, cudaMemcpyHostToDevice);
cudaMemcpy(d_b, b, tot_size, cudaMemcpyHostToDevice);

// Launch add() kernel on GPU
add<<<blocknum,threadnum>>>(d_a, d_b, d_c);

// Copy result back to host
cudaMemcpy(c, d_c, tot_size, cudaMemcpyDeviceToHost);

// Print results
printf("blocknum = %d\n", blocknum);
printf("threadnum = %d\n", threadnum);
printf("c = \n");
for(i=0; i<tot_n; i++){
printf("%d\n", c[i]);
};

// Cleanup
cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
free(a); free(b); free(c);
return 0;
}
