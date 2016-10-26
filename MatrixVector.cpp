#include <bits/stdc++.h>
#include <device_launch_parameters.h>
#include <cuda_runtime.h>
#include <cuda.h>

using namespace std;

__global__ 
void matvecMultiplication(float *vec, float *mat, float *result, const int N, const int M)
{
    int index=threadIdx.x+blockIdx.x*blockDim.x;
    float sum=0;
    if(index<M)
    {
        for(int i=0;i<N;++i)
            sum+=vec[i]*mat[(i*M)+index];
        result[index]=sum;
    }
}

int rdtsc()
{
    __asm__ __volatile__("rdtsc");
}


int main()
{
    srand(rdtsc());
    
    int N;
    int M;
    cin>>N>>M;
	
    float *a, *b, *c;
    float *d_a, *d_b, *d_c;
    
    a=new float[N];
    b=new float[N*M];
    c=new float[M];
        
    for(int i=0;i<N;++i)
    {
	a[i]=rand()%100;
	cout<<a[i]<<" ";
    }
    cout<<endl;
    
    for(int i=0;i<N;++i)
    {
	for(int j=0;j<M;++j)
	{
	    b[i*M+j]=rand()%100;
	    cout<<b[i*M+j]<<" ";
	}
	cout<<endl;
    }
    
    for(int i=0;i<N;++i)
	c[i]=0;
    

    ///Host -> Device
    cudaMalloc((void**)&d_a, sizeof(float)*N);
    cudaMalloc((void**)&d_b, sizeof(float)*N*M);
    cudaMalloc((void**)&d_c, sizeof(float)*M);
    cudaMemcpy(d_a, a, sizeof(float)*N, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof(float)*N*M, cudaMemcpyHostToDevice);

    matvecMultiplication<<<M/256+1, 256>>>(dev_a, dev_b, dev_c, N, M);

    ///Device -> Host
    cudaMemcpy(c, dev_c, sizeof(float)*M, cudaMemcpyDeviceToHost);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);


    for(int i=0;i<N;++i)
	cout<<c[i]<<" ";
    cout<<endl;

        return 0;
};
