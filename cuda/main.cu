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

__global__
void update_clusters(int k, float ** cluster, const int * cluster_assignment, const int dimensions, float  ** feature_vector,const int * cluster_size, int * response){
    response[0] = 0;

    float ** temp;
    temp = new float* [k];
    for (int i=0; i<k; i++)
        temp[i] = new float[dimensions];

    for (int i=0; i<k; i++){
        for (int j=0; j<dimensions; j++){
        temp[i][j] = (float) 0;
        }
    }  

    for (int i=0; i<data.size; i++){
        for (int j=0; j<dimensions; j++){
        temp[cluster_assignment[i]][j] += feature_vector[i][j];   
        }
    }

    printf("College :: %d \n", dimensions);

    for (int i=0; i<k; i++){
        if (cluster_size[i] == 0){
            std::printf("ZERO ::: %d \n", i);    
        // cout << "ZERO ::: " << i << endl;
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

void parse_data(const data_map &data, int &size, int &dimensions, string ** data_title, float *** data_vector){
    vector<float> sample_map_data = data.begin()->second; 
    size = data.size();   
    dimensions = sample_map_data.size();

    // cudaMallocManaged(&data_title, size*sizeof(string)); 
    // cudaMallocManaged(&data_vector, size*sizeof(float*));
    // for (int i=0; i<size; i++)
    //     cudaMallocManaged(&data_vetor[i], dimensions*sizeof(float)); 

    // int index = 0;
    // for (data_map::const_iterator it = data.begin(); it != data.end(); it++) {
    //     data_title[index] = it->first;
    //     for (int j=0; j<dimensions; j++)
    //         data_vetor[index][j] = (it->second)[j];
    //     index++;
    // } 

}


int * find_clusters(int k, const data_map data, int max_iter) {
    // int iter = 0;
    int * cluster_size;
    int * cluster_assignment;

    // college data parsing
    int data_dimensions;
    int data_size;
    string * data_title;
    float ** data_vector;
    parse_data(data, data_dimensions, data_size, &data_title, &data_vector);
    cout << "Size : " << data_size << "  dim : " << data_dimensions << endl;


    cudaMallocManaged(&cluster_size, k*sizeof(int));
    cudaMallocManaged(&cluster_assignment, data_size*sizeof(int));

    int blockSize = 256;
    int numBlocks = (data_size + blockSize - 1) / blockSize;

    init_cluster_assignment<<<numBlocks ,blockSize >>>(k, data_size, cluster_size, cluster_assignment);
    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();

    calculate_cluster_size(k, cluster_assignment, data_size, cluster_size);

    float ** cluster;
    cudaMallocManaged(&cluster, k*sizeof(float*));

    for (int i=0; i<k; i++)
        cudaMallocManaged(&cluster[i], data_dimensions*sizeof(float));

    int * did_change;
    cudaMallocManaged(&did_change, sizeof(int));

    for (int i=0; i < max_iter; i++) {
        update_clusters<<<1,1>>>(k, cluster, cluster_assignment, data_dimensions, data_vector, cluster_size, did_change);
        cudaDeviceSynchronize();

        // if (did_change[0] == 1){
        //     update_cluster_assignment(k, cluster_assignment, cluster_size, cluster, data);
        // }
        // update_cluster_assignment<<<numBlocks ,blockSize >>>(k, cluster_assignment, cluster_size, cluster, data);
        // cudaDeviceSynchronize();
                // }
                // else{    
                //     return cluster_assignment;
                // }
            }
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
  data_map data = read_csv("./datasets/College.csv");

  int k = 10; 
  int * cluster = find_clusters(k, data, 1);


//   cudaFree(x);
//   cudaFree(y);
}