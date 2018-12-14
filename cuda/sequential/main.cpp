#include "../lib/read_csv.cpp"
#include "../lib/helpers.cpp"
#include "./k_means_cuda.cpp"
using namespace std;


int main(){
  college_dataset data = read_csv_to_college_struct("../datasets/College.csv");
  
  int k = 10; 
  int * cluster = find_clusters(k, data, 100000);

  print_cluster_size(k, cluster, data.size);

  // print_cluster(k, cluster, data.size, data);
}