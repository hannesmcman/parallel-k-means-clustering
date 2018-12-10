#ifndef EUCLIDEAN_DISTANCE
#define EUCLIDEAN_DISTANCE

#include <cmath>
using namespace std;

float euclidean_distance(vector<float> x, vector<float> y) {
  float sum = 0;
  for (int i=0; i < x.size(); i++) {
    sum += pow(x[i] - y[i], 2);
  }
  return sqrt(sum);
}

#endif
