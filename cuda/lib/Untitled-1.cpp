#include <iostream>
#include <vector>
#include <unordered_map>
#include <rand>
#include "./types.cpp"

using namespace std;

// TODO: change arguments to references
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


