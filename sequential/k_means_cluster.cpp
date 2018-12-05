#include "../lib/types.cpp"
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

// TODO: change arguments to references
bool get_mean_centroid(vector<float> &centroid, const vector<string> &cluster, const data_map &dataset){
    bool changed = 0;
    vector<float> sample_map_data = dataset.begin()->second;
    vector<float> ret(sample_map_data.size(), 0.0)

    for (int i=0; i<cluster.size(); i++)
        vector<float> point_features = dataset[];
        for (int j=0; j<values.size(); i++)
            ret[j] += dataset[cluster[]][j];
    }

    int cluster_size = cluster.size();
    for (i=0; i<cluster_size; i++)
        ret[i] /= cluster_size;

    if centroid != ret{
        changed = 1;
        centroid = ret //or something like that
    }


    return changed;
}

bool update_cluster_centers(centroid_vector &centroids, const cluster_vector &clusters, const data_map &dataset){
    bool changed = 0;
    for (int i=0; i<centroids.size(); i++){
        bool centroid_changed = get_mean_centroid(centroids[i], clusters[i], dataset);
    }
    if (centroid_changed){
        changed = 1;
    }
    return changed;
}   

int find_closest_center(vector<float> item, centroid_vector centroids) {
  float minDist = FLOAT_MAX;
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

