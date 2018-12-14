#ifndef _TYPES
#define _TYPES

#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

typedef unordered_map< string, vector<float> > data_map;
typedef vector<vector<float>> centroid_vector;
typedef vector< vector<string> > cluster_vector;

typedef struct college_dataset{
    int size;
    int dimensions;
    string * names;
    float ** features;
} cluster_dataset;

#endif