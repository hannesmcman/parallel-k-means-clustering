#include <float.h>
<<<<<<< HEAD
#include "../lib/read_csv.cpp"
=======
#include "./lib/io.cpp"
>>>>>>> 081c73d7e15a80ba67712fc54785e374a78ad292
#include "../lib/types.cpp"
#include "./host/host.cu"
using namespace std;

<<<<<<< HEAD

int main(){
  data_map data = read_csv("../datasets/College.csv");

=======
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


float euclidean_distance_array(const float * x,const float * y, int n) {
    float sum = 0;
    for (int i=0; i < n; i++) {
      sum += pow(x[i] - y[i], 2);
    }
    return sqrt(sum);
}
  

// cluster assignment using randomization
// __global__
void init_cluster_assignment(int k, int size, int * cluster_size, int * cluster_assignment){
    for (int i=0; i<k; i++)
      cluster_size[i] = 0;
    int group = 0;
  
    // cluster_assignment = new int[size];
    srand(static_cast<unsigned int>(clock()));
    for (int i=0; i<size; i++){
      group = (int) rand() % k;
      cluster_assignment[i] = group;
      cluster_size[group] += 1;
    }
  }

bool update_clusters(int k, float ** cluster, const int * cluster_assignment, const cluster_dataset &data, const int * cluster_size){
bool did_change = 0;

float ** temp;
temp = new float* [k];
for (int i=0; i<k; i++)
    temp[i] = new float[data.dimensions];

for (int i=0; i<k; i++){
    for (int j=0; j<data.dimensions; j++){
    temp[i][j] = (float) 0;
    }
}  

for (int i=0; i<data.size; i++){
    for (int j=0; j<data.dimensions; j++){
    temp[cluster_assignment[i]][j] += data.features[i][j];   
    }
}

for (int i=0; i<k; i++){
    if (cluster_size[i] == 0){
    cout << "ZERO ::: " << i << endl;
    continue;
    }
    for (int j=0; j<data.dimensions; j++){
    if (cluster[i][j] != temp[i][j]/cluster_size[i]){
        did_change = 1;
    }
    cluster[i][j] = temp[i][j]/cluster_size[i];
    }
}

return did_change;
}


int find_nearest_center(int k, const float * features, int dimensions,float ** cluster){
float minDist = FLT_MAX;
int minIndex = 0;
float dist = 0;
for (int i=0; i<k; i++){
    dist = euclidean_distance_array(features, cluster[i], dimensions);
    if (dist < minDist) {
        minDist = dist;
        minIndex = i;
    }
}
return minIndex;
}

void print_cluster_assignment(int k, int *cluster_assignment,int n){
vector<int> ret[k];
for (int i=0; i<n; i++){
    ret[cluster_assignment[i]].push_back(i);
}
}

void update_cluster_assignment(int k, int * cluster_assignment, int * cluster_size, float ** cluster, const cluster_dataset &data){
for (int i=0; i<k; i++){
    cluster_size[i] = 0;
}

for (int i=0; i<data.size; i++){
    cluster_assignment[i] = find_nearest_center(k, data.features[i], data.dimensions, cluster);
    cluster_size[cluster_assignment[i]]++;
}
}


int * find_clusters(int k, const cluster_dataset data, int max_iter) {
// int iter = 0;
int * cluster_size;
int * cluster_assignment;

// cluster_size = new int[k]; 

cudaMallocManaged(&cluster_size, k*sizeof(int));
cudaMallocManaged(&cluster_assignment, data.size*sizeof(int));

init_cluster_assignment(k, data.size, cluster_size, cluster_assignment);

    float ** cluster;
    cluster = new float* [k];
    for (int i=0; i<k; i++)
        cluster[i] = new float[data.dimensions];


    for (int i=0; i < max_iter; i++) {
        // cout << iter++ << endl;
        bool did_change = update_clusters(k, cluster, cluster_assignment, data, cluster_size);
        if (did_change){
        update_cluster_assignment(k, cluster_assignment, cluster_size, cluster, data);
        }
        else{
        return cluster_assignment;
        }
    }
    return cluster_assignment;
}



// CUDA Kernel function to add the elements of two arrays on the GPU
__global__
void add(int n, float *x, float *y)
{
  for (int i = 0; i < n; i++)
      y[i] = x[i] + y[i];
}

int main(){
  data_map college_data = read_csv("./datasets/College.csv");
  cluster_dataset data = fill_college_struct(college_data);
  
>>>>>>> 081c73d7e15a80ba67712fc54785e374a78ad292
  int k = 10; 
  int * cluster = find_clusters(k, data, 100);

}