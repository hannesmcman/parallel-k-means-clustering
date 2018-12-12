#include "../lib/types.cpp"
#include "../lib/euclidean_distance.cpp"
#include "./k_means_cluster.cpp"
#include "../lib/helpers.cpp"
using namespace std;

void find_closest_center_test(int k, int numFeatures, float max) {
  vector<float> item = gen_random_vector(numFeatures, max);
  centroid_vector centroids = gen_random_centroids(k, numFeatures, max);
  cout << "Item:" << endl;
  print<float>(item);
  cout << "Centroids:" << endl;
  for (centroid_vector::iterator it = centroids.begin(); it != centroids.end(); it++) {
    cout << it - centroids.begin() << ": ";
    print<float>(*it);
  }
  cout << "Closest centroid: " << find_closest_center(item, centroids) << endl;
}


int main () {
  find_closest_center_test(2, 5, 10);
}
