#include "./k_means_cluster.cpp"
#include "../lib/read_csv.cpp"
#include "./lib/helpers.cpp"
using namespace std;

int main () {
  data_map college_data = read_csv("../datasets/College.csv");
  cluster_vector clusters = find_clusters(5, college_data, 100000);

  for (int i=0; i < clusters.size(); i++) {
    cout << i << ": ";
    print(clusters[i]);
  }
}
