#include <float.h>
#include <cstdlib>
#include "../device/device.cu"

// __global__ void
// reduce0(float* g_idata,float* g_odata, unsigned int n) {
//     extern __shared__ float temp[];
//     int thid = threadIdx.x;
//     temp[thid] = g_idata[thid];
//     __syncthreads();
//     for(int offset = 1;offset < n; offset *= 2) {
//         if(thid >= offset)
//             temp[thid] += temp[thid - offset];
//         __syncthreads();
//     }
//     g_odata[thid] = temp[thid];
// }

// cluster assignment using randomization
__global__
void init_cluster_assignment(int k, int size, int * cluster_size, int * cluster_assignment){
    for (int i=0; i<k; i++)
      cluster_size[i] = 0;
    int group = 0;

    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    int random = index % k ;

    for (int i=index; i<size; i+=stride){
      group = (int) random;
      cluster_assignment[i] = group;
      cluster_size[group] += 1;
    }
  }

__global__
void update_clusters(int k, float ** cluster, const int * cluster_assignment, int data_size, \
                int dimensions, float  ** feature_vector,const int * cluster_size, int * response){
    response[0] = 0;

    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    float ** temp;
    temp = new float* [k];
    for (int i=0; i<k; i++){
        temp[i] = new float[dimensions];
        for (int j=0; j<dimensions; j++){
            temp[i][j] = (float) 0;
        }
    }

    for (int i=0; i<data_size; i++){
        for (int j=0; j<dimensions; j++){
        temp[cluster_assignment[i]][j] += feature_vector[i][j];   
        }
    }

    for (int i=index; i<k; i+=stride){
        if (cluster_size[i] == 0){
            continue;
        }
        for (int j=0; j<dimensions; j++){
        if (cluster[i][j] != temp[i][j]/cluster_size[i]){
            response[0] = 1;
        }
        cluster[i][j] = temp[i][j]/cluster_size[i];
        }
    }
}


__global__
void update_cluster_assignment(int k, int * cluster_assignment, float ** cluster, int size, int dimension, float ** features){

    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for (int i=index; i<size; i+=stride){
        cluster_assignment[i] = find_nearest_center(k, features[i], dimension, cluster);
    }
}