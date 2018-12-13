#include "../lib/types.cpp"
#include "./lib/helpers.cpp"
#include <float.h>
#include <iostream>
// #include <thrust/device_vector>
using namespace std;

#define FLOAT_MAX 3.402823e+38

// cluster assignment using randomization
int * init_cluster_assignment(int k, int size, int * cluster_size){
  for (int i=0; i<k; i++)
    cluster_size[i] = 0;
  int group = 0;

  int * ret;
  ret = new int[size];
  srand(static_cast<unsigned int>(clock()));
  for (int i=0; i<size; i++){
    group = (int) rand() % k;
    ret[i] = group;
    cluster_size[group] += 1;
  }
  return ret;
}

bool update_clusters(int k, float ** cluster, const int * cluster_assignment, const cluster_dataset &data, const int * cluster_size){
  bool did_change = 0;

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
      continue;
    }
    for (int j=0; j<data.dimensions; j++){
      if (cluster[i][j] != temp[i][j]/cluster_size[i]){
      // cout << cluster[i][j] << "  " << temp[i][j]/cluster_size[i] << endl;
        did_change = 1;
      }
      cluster[i][j] = temp[i][j]/cluster_size[i];
    }
  }

  return did_change;
}


int find_nearest_center(int k, const float * features, int dimensions,float ** cluster){
  float minDist = FLOAT_MAX;
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

void print_cluster_assignment(int k, int *cluster_assignment,int n){
  vector<int> ret[k];
  for (int i=0; i<n; i++){
    ret[cluster_assignment[i]].push_back(i);
  }
}

void update_cluster_assignment(int k, int * cluster_assignment, int * cluster_size, float ** cluster, const cluster_dataset &data){
  for (int i=0; i<k; i++){
    cluster_size[i] = 0;
  }

  for (int i=0; i<data.size; i++){
    cluster_assignment[i] = find_nearest_center(k, data.features[i], data.dimensions, cluster);
    cluster_size[cluster_assignment[i]]++;
  }
}

int * find_clusters(int k, const cluster_dataset data, int max_iter, int &num_iter) {
  int * cluster_size;
  cluster_size = new int[k];
  int * cluster_assignment;

  float ** cluster;
  cluster = new float* [k];
  for (int i=0; i<k; i++)
    cluster[i] = new float[data.dimensions];

  cluster_assignment = init_cluster_assignment(k, data.size, cluster_size);

  for (int i=0; i < max_iter; i++) {
    // cout << iter++ << endl;
    num_iter++;
    bool did_change = update_clusters(k, cluster, cluster_assignment, data, cluster_size);
    if (did_change){
      update_cluster_assignment(k, cluster_assignment, cluster_size, cluster, data);
    }
    else{
      return cluster_assignment;
    }
    // cout << "Iteration: " << i << endl;
  }
  return cluster_assignment;
}
