#ifndef _HELPERS
#define _HELPERS

#include "../../lib/types.cpp"
#include <vector>
using namespace std;

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
