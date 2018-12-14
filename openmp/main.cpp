#include "../lib/io.cpp"
#include "../sequential/lib/helpers.cpp"
#include "./openmp_cluster.cpp"
#include <string>
#include <sstream>
using namespace std;


void print_cluster(int k, int *cluster_assignment,int n,const cluster_dataset& data){
  vector<int> ret[k];
  for (int i=0; i<n; i++){
    ret[cluster_assignment[i]].push_back(i);
  }
  for (int i=0; i<k; i++){
    cout << i << " ::: ";
    for (int j=0; j<ret[i].size(); j++)
      cout << data.names[ret[i][j]] << ", ";
    cout << endl << endl;
  }
}

int main(int argc, char *argv[]){
  string dataset_name(argv[1]);
  int k = atoi(argv[2]);
  int max_iter = atoi(argv[3]);

  stringstream filename;
  filename << "../datasets/" << dataset_name;

  data_map input_data = read_csv(filename.str());
  cluster_dataset data = fill_college_struct(input_data);
  int num_iter = 0;
  int * cluster = find_clusters(k, data, max_iter, num_iter);

  // print_cluster(k, cluster, data.size, data);
  cout << "Stopped after " << num_iter << " iterations." << endl;
}
