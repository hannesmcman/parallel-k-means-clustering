#include "../lib/types.cpp"
#include "../lib/euclidean_distance.cpp"
#include "../lib/helpers.cpp"
#include <cfloat>
#include <iostream>
// #include <thrust/device_vector>
using namespace std;

vector<int> * init_cluster_centers(int k, const college_dataset& data, int * cluster_size){
  vector<int> * ret;
  ret = new vector<int>[k];
  for (int i=0; i<data.size; i++){
    srand(static_cast<unsigned int>(clock()));
    ret[(int) rand() % k] .push_back(i);
  }  
  for (int i=0; i<k; i++)
    cluster_size[i] = ret[i].size();
  return ret;
}

int * init_cluster_assignment(int k, int size, int * cluster_size){
  for (int i=0; i<k; i++)
    cluster_size[i] = 0;
  int group = 0;

  int * ret;
  ret = new int[size];
  srand(static_cast<unsigned int>(clock()));
  for (int i=0; i<size; i++){
    group = (int) rand() % k;
    ret[i] = group;
    cluster_size[group] += 1;
  }
  return ret;
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

// int find_closest_center(vector<float> item, centroid_vector centroids) {
//   float minDist = FLT_MAX;
//   int minIndex = 0;
//   for (centroid_vector::iterator it = centroids.begin(); it != centroids.end(); it++) {
//     const float dist = euclidean_distance(item, *it);
//     if (dist < minDist) {
//       minDist = dist;
//       minIndex = it - centroids.begin();
//     }
//   }
//   return minIndex;
// }

bool get_cluster(int k, float ** cluster, const int * cluster_assignment, const college_dataset &data, const int * cluster_size){
  bool did_change = 0;

  float ** temp;
  temp = new float* [k];
  for (int i=0; i<k; i++)
    temp[i] = new float[data.dimensions];

  for (int i=0; i<k; i++){
    for (int j=0; j<data.dimensions; j++){
      temp[i][j] = (float) 0;
    }
  }  

  for (int i=0; i<data.size; i++){
    for (int j=0; j<data.dimensions; j++){
      temp[cluster_assignment[i]][j] += data.features[i][j];   
    }
  }

  for (int i=0; i<k; i++){
    if (cluster_size[i] == 0)
      continue;
    for (int j=0; j<data.dimensions; j++){
      if (cluster[i][j] != temp[i][j]/cluster_size[i]){
      // cout << cluster[i][j] << "  " << temp[i][j]/cluster_size[i] << endl;
        did_change = 1;
      }
      cluster[i][j] = temp[i][j]/cluster_size[i];
    }
  }

  return did_change;
}


int find_nearest_center(int k, const float * features, int dimensions,float ** cluster){
  float minDist = FLT_MAX;
  int minIndex = 0;
  float dist = 0;
  for (int i=0; i<k; i++){
    dist = euclidean_distance_array(features, cluster[i], dimensions);
    if (dist < minDist) {
        minDist = dist;
        minIndex = i;
      }
  }
  return minIndex;
}


void print_cluster_assignment(int k, int *cluster_assignment,int n){
  vector<int> ret[k];
  for (int i=0; i<n; i++){
    ret[cluster_assignment[i]].push_back(i);
  }
}


void update_cluster_assignment(int k, int * cluster_assignment, int * cluster_size, float ** cluster, const college_dataset &data){
  for (int i=0; i<k; i++){
    cluster_size[i] = 0;
  }

  for (int i=0; i<data.size; i++){
    cluster_assignment[i] = find_nearest_center(k, data.features[i], data.dimensions, cluster);
    cluster_size[cluster_assignment[i]]++;
  }
}

int * find_clusters(int k, const college_dataset data, int max_iter) {
  int iter = 0;
  int * cluster_size;
  cluster_size = new int[k]; 
  int * cluster_assignment;
  
  float ** cluster;
  cluster = new float* [k];
  for (int i=0; i<k; i++)
    cluster[i] = new float[data.dimensions];
  
  cluster_assignment = init_cluster_assignment(k, data.size, cluster_size);

  for (int i=0; i < max_iter; i++) {
    bool did_change = get_cluster(k, cluster, cluster_assignment, data, cluster_size);
    if (did_change){
      update_cluster_assignment(k, cluster_assignment, cluster_size, cluster, data);
    }
    else{
      return cluster_assignment;
    }
  }
  return cluster_assignment;
}

