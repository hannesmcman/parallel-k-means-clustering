#include "../lib/types.cpp"
#include "../lib/euclidean_distance.cpp"
#include "../lib/helpers.cpp"
#include <cfloat>
#include <iostream>
using namespace std;

centroid_vector init_cluster_centers(int k_length, const data_map &dataset){
    vector<float> sample_map_data = dataset.begin()->second;
    int num_features = sample_map_data.size();
    vector<float> min_map_data = dataset.begin()->second;
    vector<float> max_map_data = dataset.begin()->second;
    for (data_map::const_iterator it = dataset.begin(); it != dataset.end(); it++) {
      for (int i=0; i<num_features; i++){
        if (it->second[i] < min_map_data[i]){
          min_map_data[i] = it->second[i];
        }
        if (it->second[i] > max_map_data[i]){
          max_map_data[i] = it->second[i];
        }
      }
    }
    centroid_vector centroids =  gen_random_centroids(k_length, num_features , min_map_data, max_map_data);
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
