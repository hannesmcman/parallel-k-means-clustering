#include <float.h>
#include <cstdlib>
#include "./lib/read_csv.cpp"
#include "../lib/types.cpp"
#include "./lib/helpers.cu"
using namespace std;

float euclidean_distance_array(const float * x,const float * y, int n) {
    float sum = 0;
    for (int i=0; i < n; i++) {
      sum += pow(x[i] - y[i], 2);
    }
    return sqrt(sum);
}
  

void calculate_cluster_size(int k, int *cluster_assignment,int n, int * cluster_size){
    for (int i=0; i<k; i++)
      cluster_size[i] = 0;
    for (int i=0; i<n; i++){
      cluster_size[cluster_assignment[i]]++;
    }
  }

// cluster assignment using randomization
__global__
void init_cluster_assignment(int k, int size, int * cluster_size, int * cluster_assignment){
    for (int i=0; i<k; i++)
      cluster_size[i] = 0;
    int group = 0;

    // srand(static_cast<unsigned int>(clock()));
    
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    int random = index % k ;

    // int random = curandGenerate();
    // std::printf("block id : %d \t block dim: %d \t thread id : %d \t index : %d \t random : %d \n",blockIdx.x, blockDim.x, threadIdx.x, index, random);

    // cluster_assignment = new int[size];
    for (int i=index; i<size; i+=stride){
      group = (int) random;
    //   group = (int) (rand() % k);
      cluster_assignment[i] = group;
      cluster_size[group] += 1;
    }
  }

bool update_clusters(int k, float ** cluster, const int * cluster_assignment, const college_dataset &data, const int * cluster_size, bool &response){
    response = 0;

    float ** temp;
    temp = new float* [k];
    for (int i=0; i<k; i++)
        temp[i] = new float[data.dimensions];

    for (int i=0; i<k; i++){
        for (int j=0; j<data.dimensions; j++){
        temp[i][j] = (float) 0;
        }
    }  

    for (int i=0; i<data.size; i++){
        for (int j=0; j<data.dimensions; j++){
        temp[cluster_assignment[i]][j] += data.features[i][j];   
        }
    }

    for (int i=0; i<k; i++){
        if (cluster_size[i] == 0){
        cout << "ZERO ::: " << i << endl;
        continue;
        }
        for (int j=0; j<data.dimensions; j++){
        if (cluster[i][j] != temp[i][j]/cluster_size[i]){
            response = 1;
        }
        cluster[i][j] = temp[i][j]/cluster_size[i];
        }
    }
}

int find_nearest_center(int k, const float * features, int dimensions,float ** cluster){
    float minDist = FLT_MAX;
    int minIndex = 0;
    float dist = 0;
    for (int i=0; i<k; i++){
        dist = euclidean_distance_array(features, cluster[i], dimensions);
        if (dist < minDist) {
            minDist = dist;
            minIndex = i;
        }
    }
    return minIndex;
}

// __global__
void update_cluster_assignment(int k, int * cluster_assignment, int * cluster_size, float ** cluster, const college_dataset &data){
    for (int i=0; i<k; i++){
        cluster_size[i] = 0;
    }

    for (int i=0; i<data.size; i++){
        cluster_assignment[i] = find_nearest_center(k, data.features[i], data.dimensions, cluster);
        cluster_size[cluster_assignment[i]]++;
    }
}


int * find_clusters(int k, const college_dataset data, int max_iter) {
    int iter = 0;
    int * cluster_size;
    int * cluster_assignment;

    // cluster_size = new int[k]; 

    cudaMallocManaged(&cluster_size, k*sizeof(int));
    cudaMallocManaged(&cluster_assignment, data.size*sizeof(int));

    int blockSize = 256;
    int numBlocks = (data.size + blockSize - 1) / blockSize;

    init_cluster_assignment<<<numBlocks ,blockSize >>>(k, data.size, cluster_size, cluster_assignment);
    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();

    calculate_cluster_size(k, cluster_assignment, data.size, cluster_size);

    print_cluster_size(k,cluster_assignment,data.size);

    float ** cluster;
    cudaMallocManaged(&cluster, k*sizeof(float*));

    // cluster = new float* [k];
    for (int i=0; i<k; i++)
        cudaMallocManaged(&cluster[i], data.dimensions*sizeof(float));
        // cluster[i] = new float[data.dimensions];

    bool did_change;
    cudaMallocManaged(did_change, sizeof(int));


    for (int i=0; i < max_iter; i++) {
        cout << "iteration : " << iter++ << endl; 
        update_clusters(k, cluster, cluster_assignment, data, cluster_size, did_change);

        for (int i=0; i<k; i++)
            cout << i << ":" << cluster_size[i] << endl;

        if (did_change){
            cout << "did change : " << iter << endl;
            update_cluster_assignment(k, cluster_assignment, cluster_size, cluster, data);

        // update_cluster_assignment<<<numBlocks ,blockSize >>>(k, cluster_assignment, cluster_size, cluster, data);
        // cudaDeviceSynchronize();
        }
        else{    
            print_cluster_size(k,cluster_assignment,data.size);
            return cluster_assignment;
        }
        iter++;
    }
    print_cluster_size(k, cluster_assignment, data.size);
    return cluster_assignment;
}


// // CUDA Kernel function to add the elements of two arrays on the GPU
// __global__
// void add(int n, float *x, float *y)
// {
//   int index = blockIdx.x * blockDim.x + threadIdx.x;
//   int stride = blockDim.x * gridDim.x;
//   for (int i = index; i < n; i += stride)
//     y[i] =i;
// }





int main(){
  data_map college_data = read_csv("./datasets/College.csv");
  college_dataset data = fill_college_struct(college_data);
  
  int k = 10; 
  int * cluster = find_clusters(k, data, 100);

  print_cluster_size(k, cluster, data.size);
//   int N = 1<<20;
//   float *x, *y;

//   // Allocate Unified Memory – accessible from CPU or GPU
//   cudaMallocManaged(&x, N*sizeof(float));
//   cudaMallocManaged(&y, N*sizeof(float));

//   // initialize x and y arrays on the host
//   for (int i = 0; i < N; i++) {
//     x[i] = 1.0f;
//     y[i] = 2.0f;
//   }

//   // Run kernel on 1M elements on the GPU
//   int blockSize = 256;
//   int numBlocks = (N + blockSize - 1) / blockSize;

//   add<<<numBlocks, blockSize>>>(N, x, y);

//   // Wait for GPU to finish before accessing on host
//   cudaDeviceSynchronize();

//   // Check for errors (all values should be 3.0f)
//   float maxError = 0.0f;
//   for (int i = 0; i < N; i++)
//     maxError = std::fmax(maxError, fabs(y[i]-3.0f));
//   cout << "Max error: " << maxError << std::endl;

//   // Free memory
//   cudaFree(x);
//   cudaFree(y);
}