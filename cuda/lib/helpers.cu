#include <iostream>
#include <float.h>

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
  
  void print_cluster_size(int k, int *cluster_assignment,int n){
    int size[k];
    for (int i=0; i<k; i++)
      size[i] = 0;
    for (int i=0; i<n; i++){
      size[cluster_assignment[i]]++;
    }
    for (int i=0; i<k; i++){
      cout << i << " ::: " << size[i] << endl;
    }
  }

  