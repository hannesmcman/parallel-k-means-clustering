#include "../lib/types.cpp"
#include "../lib/euclidean_distance.cpp"
#include "../lib/helpers.cpp"
#include "../sequential/lib.cpp"
#include <cfloat>
#include <iostream>
#include <omp.h>
using namespace std;

cluster_vector find_clusters(int k, data_map data, int max_iter) {
  centroid_vector cluster_centers = init_cluster_centers(k, data);
  cluster_vector clusters(k);
  for (int i=0; i < max_iter; i++) {
    clusters = cluster_vector(k);
#pragma openmp parallel for default(shared)
    for (data_map::iterator it = data.begin(); it != data.end(); it++) {
      int closest_center = find_closest_center(it->second, cluster_centers);
      clusters[closest_center].push_back(it->first);
    }
    bool didChange = update_cluster_centers(cluster_centers, clusters, data);
    if (!didChange) {
      cout << "Finished after " << i << " iterations." << endl;
      return clusters;
    }
    cout << "Iteration: " << i << endl;
  }
  return clusters;
}
