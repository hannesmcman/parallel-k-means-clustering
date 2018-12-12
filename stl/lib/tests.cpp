#include <iostream>
#include "./read_csv.cpp"
#include "./euclidean_distance.cpp"
using namespace std;

void read_csv_test () {
  data_map map = read_csv("../datasets/College.csv");
  for (data_map::iterator it = map.begin(); it != map.end(); it++) {
    cout << it->first << ": ";
    for (vector<float>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
      cout << *it2 << " ; ";
    }
    cout << endl;
  }
}

void euclidean_distance_test () {
  vector<float> a = { 3.5, 2.5, 3, 4, 5, 6};
  vector<float> b = { 1, 2, 3, 8, 5, 1};
  cout << euclidean_distance(a, b) << endl;
}

int main () {
  euclidean_distance_test();
}
