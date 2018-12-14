#ifndef _CUDA_HELPERS
#define _CUDA_HELPERS

#include <iostream>
#include "../../lib/types.cpp"
#include <time.h>
#include <random>
#include <string>
using namespace std;

void print_cluster(int k, int *cluster_assignment,int data_size,string * data_titles){
    vector<int> ret[k];
    for (int i=0; i<data_size; i++){
      ret[cluster_assignment[i]].push_back(i);
    }
    for (int i=0; i<k; i++){
      cout << i << " ::: ";
      for (int j=0; j<ret[i].size(); j++)
        cout << data_titles[ret[i][j]] << ", ";
      cout << endl << endl;
    }
  }
  void print_cluster_size(int k, int *cluster_assignment,int data_size){
      int size[k];
      for (int i=0; i<k; i++)
        size[i] = 0;
      for (int i=0; i<data_size; i++){
        size[cluster_assignment[i]]++;
      }
      for (int i=0; i<k; i++){
        cout << i << " ::: " << size[i] << endl;
      }
    }

#endif
