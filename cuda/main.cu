#include "../lib/read_csv.cpp"
#include "../lib/types.cpp"
#include "./cuda_lib/helpers.cu"
#include "./device/device.cu"
#include "./host/host.cu"
#include "./global/global.cu"
using namespace std;


int main(){
  data_map data = read_csv("../datasets/College.csv");

  int k = 10; 
  int * cluster = find_clusters(k, data, 1);

}