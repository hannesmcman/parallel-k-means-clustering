#include "../lib/read_csv.cpp"
#include "../sequential/lib/helpers.cpp"
#include "./k_means_cluster.cpp"
using namespace std;

void print_cluster(int k, int *cluster_assignment,int n,const college_dataset& data){
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

int main(){
  data_map college_data = read_csv("../datasets/College.csv");
  // data_map college_data = read_csv("../datasets/list_landmarks_align_celeba.csv");
  // data_map college_data = read_csv("../datasets/list_attr_celeba.csv");
  college_dataset data = fill_college_struct(college_data);

  int k = 5;
  int * cluster = find_clusters(k, data, 100);

  print_cluster(k, cluster, data.size, data);
}