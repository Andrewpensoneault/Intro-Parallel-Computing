#include <stdio.h>
#include <cuda_runtime.h>
//Device code
__global__
void cuda_kernel()
{
    printf("Hello, World! \n");
}
//Host code
int main()
{
    cuda_kernel <<< 1, 1 >>> ();
    cudaDeviceSynchronize();
    cudaDeviceReset();
    return 0;
}