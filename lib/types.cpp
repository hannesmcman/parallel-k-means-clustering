#ifndef _TYPES
#define _TYPES

#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

typedef unordered_map< string, vector<float> > data_map;
typedef vector<vector<float>> centroid_vector;
typedef vector< vector<string> > cluster_vector;

<<<<<<< HEAD
typedef struct college_dataset{
=======
typedef struct cluster_dataset{
>>>>>>> 081c73d7e15a80ba67712fc54785e374a78ad292
    int size;
    int dimensions;
    string * names;
    float ** features;
} cluster_dataset;

<<<<<<< HEAD
#endif
=======
#endif
>>>>>>> 081c73d7e15a80ba67712fc54785e374a78ad292
