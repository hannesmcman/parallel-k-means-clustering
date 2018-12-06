#include "../lib/types.cpp"
#include "../lib/euclidean_distance.cpp"
#include <cfloat>
using namespace std;

centroid_vector init_cluster_centers(int k_length, const data_map &dataset){
    centroid_vector centroids;
    vector<float> sample_map_data = dataset.begin()->second;
    for (int i=0; i<k_length; i++){
        for (int j=0; j<sample_map_data.size(); j++)
            centroids[i].push_back(rand());
    }
    return centroids;
}

bool get_mean_centroid(vector<float> &centroid, const vector<string> &cluster, const data_map &dataset){
    bool changed = 0;
    vector<float> sample_map_data = dataset.begin()->second;
    vector<float> ret(sample_map_data.size(), 0.0);

    for (vector<string>::const_iterator it = cluster.begin(); it != cluster.end(); it++) {
    // for (int i=0; i<cluster.size(); i++)
        // vector<float> point_features = dataset[cluster[i]];
        vector<float> point_features = dataset.at(*it);
        for (int j=0; j<point_features.size(); j++)
            ret[j] += point_features[j];
    }

    int cluster_size = cluster.size();
    for (int i=0; i<ret.size(); i++)
        ret[i] /= cluster_size;

    if (centroid != ret) {
        changed = 1;
        for (int i=0; i<ret.size(); i++)
          centroid[i] = ret[i];
    }
    return changed;
}

bool update_cluster_centers(centroid_vector &centers, const cluster_vector &clusters, const data_map &dataset){
    bool changed = 0;
    bool centroid_changed = 0;
    for (centroid_vector::iterator it = centers.begin(); it != centers.end(); it++) {
      int index = it - centers.begin();
      centroid_changed = get_mean_centroid(*it, clusters[index], dataset);
    }
    // for (int i=0; i<centers.size(); i++){
    //     bool centroid_changed = get_mean_centroid(centers[i], clusters[i], dataset);
    // }
    if (centroid_changed){
        changed = 1;
    }
    return changed;
}

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
