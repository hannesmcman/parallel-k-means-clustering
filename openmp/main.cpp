#include "./k_means_cluster.cpp"
#include "../lib/read_csv.cpp"
#include "../lib/helpers.cpp"
using namespace std;

int main (int argc, char *argv[]) {
  string filename = string(argv[1]);
  int num_clusters = atoi(argv[2]);
  int max_iterations = atoi(argv[3]);
  data_map face_data = read_csv(filename);
  cluster_vector clusters = find_clusters(num_clusters, face_data, max_iterations);

  for (int i=0; i < clusters.size(); i++) {
    cout << i << ": ";
    for (int j=0; j < 5 && j < clusters[i].size(); j++) {
      cout << clusters[i][j] << ", ";
    }
    cout << endl;
    // print(clusters[i].size);
  }
}
