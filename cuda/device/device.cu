__device__
float euclidean_distance_array(const float * x,const float * y, int n) {
    float sum = 0;
    for (int i=0; i < n; i++) {
      sum += pow(x[i] - y[i], 2);
    }
    return sqrt(sum);
}

__device__
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

