#include <iostream>
#include "./read_csv.cpp"
using namespace std;

int main () {
  data_map map = readCSV("../datasets/College.csv");
  for (data_map::iterator it = map.begin(); it != map.end(); it++) {
    cout << it->first << ": ";
    for (vector<float>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
      cout << *it2 << " ; ";
    }
    cout << endl;
  }
}
