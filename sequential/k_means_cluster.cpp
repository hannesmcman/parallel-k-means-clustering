#include "../lib/types.cpp"
#include <cfloat>
using namespace std;

int find_closest_center(vector<float> item, centroid_vector centroids) {
  float minDist = FLT_MAX;
  int minIndex = 0;
  for (centroid_vector::iterator it = centroids.begin(); it != centroids.end(); it++) {
    const float dist = euclidean_distance(item, *it);
    if (dist < minDist) {
      minDist = dist;
      minIndex = it - centroids.begin();
    }
  }
  return minIndex;
}

cluster_vector find_clusters(int k, data_map data, int max_iter) {
  centroid_vector cluster_centers = init_cluster_centers(k, data);
  cluster_vector clusters(k);
  for (int i=0; i < max_iter; i++) {
    clusters = cluster_vector(k);
    for (data_map::iterator it = data.begin(); it != data.end(); it++) {
      int closest_center = find_closest_center(it->second, cluster_centers);
      clusters[closest_center].push_back(it->first);
    }

    bool didChange = update_cluster_centers(cluster_centers, clusters, data);
    if (!didChange) {
      return clusters;
    }
  }
  return clusters;
}
