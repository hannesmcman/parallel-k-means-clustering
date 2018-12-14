#ifndef _HELPERS
#define _HELPERS

#include <iostream>
#include "./types.cpp"
#include <time.h>
#include <random>
using namespace std;

template<class T>
void print(const vector<T> &vec) {
  for (int i=0; i < vec.size(); i++) {
    cout << vec[i] << " ";
  }
  cout << endl;
}

vector<float> gen_random_vector(int size, vector<float> min, vector<float> max) {
  vector<float> ret;
    for (int i=0; i < size; i++) {
      int begin = min[i];
      int end = max[i];
      srand(static_cast<unsigned int>(clock()));
      ret.push_back((float) (rand() % ((end - begin) + 1) + begin));
  }
  return ret;
}

centroid_vector gen_random_centroids(int k, int numFeatures, vector<float> min, vector<float> max) {
  centroid_vector centroids;
  for (int i=0; i < k; i++) {
    centroids.push_back(gen_random_vector(numFeatures, min, max));
  }
  return centroids;
}


void print_cluster(int k, int *cluster_assignment,int n,const college_dataset& data){
  vector<int> ret[k];
  for (int i=0; i<n; i++){
    ret[cluster_assignment[i]].push_back(i);
  }
  for (int i=0; i<k; i++){
    cout << i << " ::: ";
    for (int j=0; j<ret[i].size(); j++)
      cout << data.names[ret[i][j]] << ", ";
    cout << endl << endl;
  }
}

void print_cluster_size(int k, int *cluster_assignment,int n){
    int size[k];
    for (int i=0; i<k; i++)
      size[i] = 0;
    for (int i=0; i<n; i++){
      size[cluster_assignment[i]]++;
    }
    for (int i=0; i<k; i++){
      cout << i << " ::: " << size[i] << endl;
    }
  }



float euclidean_distance_array(const float * x,const float * y, int n) {
  float sum = 0;
  for (int i=0; i < n; i++) {
    sum += pow(x[i] - y[i], 2);
  }
  return sqrt(sum);
}


#endif