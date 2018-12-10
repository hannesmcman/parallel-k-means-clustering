#include "../lib/types.cpp"
#include "../lib/euclidean_distance.cpp"
#include "../lib/helpers.cpp"
#include "../sequential/lib.cpp"
#include <cfloat>
#include <iostream>
#include <omp.h>
using namespace std;

void order_data(data_map unordered_data, vector< vector<float> > &ordered_data, vector<string> data_labels) {
  for (data_map::iterator it = unordered_data.begin(); it != unordered_data.end(); it++) {
    ordered_data.push_back(it->second);
    data_labels.push_back(it->first);
  }
}

cluster_vector find_clusters(int k, data_map data, int max_iter) {
  centroid_vector cluster_centers = init_cluster_centers(k, data);
  cluster_vector clusters(k);
  vector< vector<float> > ordered_data;
  vector<string> data_labels;
  order_data(unordered_data, ordered_data, data_labels);
  for (int i=0; i < max_iter; i++) {
    clusters = cluster_vector(k);
#pragma omp parallel for default(shared)
    for (unsigned i=0; i < ordered_data.size(); i++) {
      int closest_center = find_closest_center(ordered_data[i], cluster_centers);
      clusters[closest_center].push_back(data_labels[i]);
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
