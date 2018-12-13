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

college_dataset fill_college_struct(const data_map& college_data){

  vector<float> sample_map_data = college_data.begin()->second; 

  college_dataset data;
  data.size = college_data.size();
  data.dimensions = sample_map_data.size();
  data.names = new string[data.size];
  data.features = new float *[data.size];
  for (int i=0; i<data.size; i++)
    data.features[i] = new float[data.dimensions];

  int index = 0;
  for (data_map::const_iterator it = college_data.begin(); it != college_data.end(); it++) {
      data.names[index] = it->first;
      for (int j=0; j<data.dimensions; j++)
        data.features[index][j] = (it->second)[j];
      index++;
  } 
  return data;
}

float euclidean_distance_array(const float * x,const float * y, int n) {
  float sum = 0;
  for (int i=0; i < n; i++) {
    sum += pow(x[i] - y[i], 2);
  }
  return sqrt(sum);
}


#endif