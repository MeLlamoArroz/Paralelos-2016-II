#include <bits/stdc++.h>
#include <device_launch_parameters.h>
#include <cuda_runtime.h>
#include <cuda.h>

using namespace std;

__global__
void myKernel1(float* __restrict__ x, float lambda, const int N)
{
    int tid = threadIdx.x + blockIdx.x * blockDim.x;

    if (tid < N)
    {
        float xa = fabs(x[tid]); 
        if(xa > lambda)
	    x[tid]=x[tid]*((xa - lambda) / xa);
	else
	    x[tid]=0;
    }

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
    int lambda;
    cin>>n;
    cin>>lambda;

    float* A =new float[n*n];
    
    for(int i=0;i<n*n;++i)
	A[i]=rand()%100;
    
    int size = n * n * sizeof(float);
    float *d_A;

    ///Host -> Device
    cudaMalloc((void **)&d_A, size);
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);

    
    dim3 DimGrid ((n-1)/256+1,1,1);
    dim3 DimBlock (256,1,1);
    myKernel1 <<<DimGrid,DimBlock  >>> (d_A, lambda, n);
    
    
    ///Device -> Host
    cudaMemcpy(A, d_A, size, cudaMemcpyDeviceToHost);
    show(A,n,n);

    cudaFree(d_A);

    system("PAUSE");
    exit(0);
}