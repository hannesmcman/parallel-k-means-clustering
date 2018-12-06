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

vector<float> gen_random_vector(int size, int max) {
  vector<float> ret;
  for (int i=0; i < size; i++) {
    srand(static_cast<unsigned int>(clock()));
    ret.push_back((float) (rand() % max));
  }
  return ret;
}

centroid_vector gen_random_centroids(int k, int numFeatures, int max) {
  centroid_vector centroids;
  for (int i=0; i < k; i++) {
    centroids.push_back(gen_random_vector(numFeatures, max));
  }
  return centroids;
}
