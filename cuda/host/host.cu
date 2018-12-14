#include <float.h>
#include <string>
#include "../../lib/types.cpp"
#include "../cuda_lib/helpers.cu"
#include "../global/global.cu"

void calculate_cluster_size(int k, int *cluster_assignment,int n, int * cluster_size){
    for (int i=0; i<k; i++)
      cluster_size[i] = 0;
    for (int i=0; i<n; i++){
      cluster_size[cluster_assignment[i]]++;
    }
  }
  
void parse_data(const data_map &data, int &size, int &dimensions, string ** data_title, float *** data_features){
    vector<float> sample_map_data = data.begin()->second; 
    size = data.size();   
    dimensions = sample_map_data.size();

    cudaMallocManaged(data_title, size*sizeof(string)); 
    cudaMallocManaged(data_features, size*sizeof(float*));
    float ** data_v = *data_features;

    for (int i=0; i<size; i++)
        cudaMallocManaged(&data_v[i], dimensions*sizeof(float)); 

    int index = 0;
    for (data_map::const_iterator it = data.begin(); it != data.end(); it++) {
        (*data_title)[index] = it->first;
        for (int j=0; j<dimensions; j++)
            data_v[index][j] = (it->second)[j];
        index++;
    } 
}

void init_clusters(int k, int size, int dimensions, float ** features, float ** cluster){
    srand(static_cast<unsigned int>(clock()));
    for (int i=0; i<k; i++){
        int index = (int) rand() % size;
        for (int j=0; j<dimensions; j++){
            cluster[i][j] = features[index][j];
        }
    }
}


int * find_clusters(int k, const data_map data, int max_iter) {
    // int iter = 0;
    int * cluster_size;
    int * cluster_assignment;

    // college data parsing
    int data_dimensions;
    int data_size;
    string * data_title;
    float ** data_features;
    parse_data(data, data_size, data_dimensions, &data_title, &data_features);

    cudaMallocManaged(&cluster_size, k*sizeof(int));
    cudaMallocManaged(&cluster_assignment, data_size*sizeof(int));

    int blockSize = 256;
    int numBlocks = (data_size + blockSize - 1) / blockSize;

    // init_cluster_assignment<<<numBlocks ,blockSize >>>(k, data_size, cluster_size, cluster_assignment);
    // Wait for GPU to finish before accessing on host
    // cudaDeviceSynchronize();

    // calculate_cluster_size(k, cluster_assignment, data_size, cluster_size);

    float ** cluster;
    cudaMallocManaged(&cluster, k*sizeof(float*));

    for (int i=0; i<k; i++)
        cudaMallocManaged(&cluster[i], data_dimensions*sizeof(float));

    int * did_change;
    cudaMallocManaged(&did_change, sizeof(int));

    //make clusters
    init_clusters(k, data_size, data_dimensions,data_features, cluster);
    update_cluster_assignment<<<numBlocks ,blockSize>>>(k, cluster_assignment, cluster_size, cluster, data_size, data_dimensions, data_features);
    cudaDeviceSynchronize();    

    calculate_cluster_size(k, cluster_assignment, data_size, cluster_size); 

    for (int i=0; i < max_iter; i++) {
        update_clusters<<<numBlocks ,blockSize>>>(k, cluster, cluster_assignment, data_size, data_dimensions, data_features, cluster_size, did_change);
        cudaDeviceSynchronize();

        if (did_change[0] == 1){
            // update_cluster_assignment(k, cluster_assignment, cluster_size, cluster, data);
            update_cluster_assignment<<<numBlocks ,blockSize>>>(k, cluster_assignment, cluster_size, cluster, data_size, data_dimensions, data_features);
            cudaDeviceSynchronize();
                }
        else{    
            print_cluster_size(k, cluster_assignment,data_size);
            return cluster_assignment;
        }
            }

    print_cluster_size(k, cluster_assignment,data_size);
    return cluster_assignment;
}
