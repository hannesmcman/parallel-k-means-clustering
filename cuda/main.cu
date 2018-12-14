#include <float.h>
#include "../lib/read_csv.cpp"
#include "../lib/types.cpp"
#include "./host/host.cu"
using namespace std;


int main(){
  data_map data = read_csv("../datasets/College.csv");

  int k = 10; 
  int * cluster = find_clusters(k, data, 1);

}