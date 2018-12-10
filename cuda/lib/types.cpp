#ifndef _TYPES
#define _TYPES

#include <string>
using namespace std;

typedef struct college_dataset{
    int size;
    int dimensions;
    string * names;
    float ** features; 
} college_dataset;

#endif