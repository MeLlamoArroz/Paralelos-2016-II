#include <bits/stdc++.h>
#include <device_launch_parameters.h>
#include <cuda_runtime.h>
#include <cuda.h>

using namespace std;

__global__
void matrixAddition(float* A, float* B, float* C, int n)
{
	int index = threadIdx.x + blockDim.x * blockIdx.x;
	if (i < n*n)
	    C[index] = A[index] + B[index];
}

void show(float* matriz, float fila, float columna)
{
    for (int i=0;i<fila;++i)
    {
	for (int j=0;j<columna;++j)
	    cout<<i*columna +j<<" ";
	cout<<endl;
    }
}

int rdtsc()
{
    __asm__ __volatile__("rdtsc");
}
  
int main()
{
    rand(rdtsc());
    
    int n;
    cin>>n;

    float* A =new float[n*n];
    float* B =new float[n*n];
    float* C =new float[n*n];
    
    for(int i=0;i<n*n;++i)
    {
	A[i]=rand()%100;
	B[i]=rand()%100;
    }
    
    int size = n * n * sizeof(float);
    float *d_A, *d_B, *d_C;

    ///Host -> Device
    cudaMalloc((void **)&d_A, size);
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMalloc((void **)&d_B, size);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
    cudaMalloc((void **)&d_C, size);

    
    dim3 DimGrid ((n-1)/256+1,1,1);
    dim3 DimBlock (256,1,1);
    matrixAddition <<<DimGrid,DimBlock  >>> (d_A, d_B, d_C, n);
    
    
    ///Device -> Host
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
    show(C,n,n);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    system("PAUSE");
    exit(0);
}