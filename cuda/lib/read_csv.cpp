#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "./types.cpp"
using namespace std;

data_map read_csv(string filename) {
  data_map final_map;
  ifstream inputFile;
  inputFile.open(filename);
  string labels;
  string line;
  getline(inputFile, labels);
  if (inputFile.is_open()) {
    while (getline(inputFile, line)) {
      vector<string> entries;
      stringstream ss(line);
      while (ss.good()) {
        string substr;
        getline( ss, substr, ',');
        entries.push_back(substr);
      }
      string label = entries[0];
      label.erase(remove(label.begin(), label.end(), '\"'), label.end());
      final_map[label] = vector<float>();
      for (int i=1; i < entries.size(); i++) {
        final_map[label].push_back(stof(entries[i]));
      }
    }
    inputFile.close();
  } else {
    throw invalid_argument("Unable to open file");
  }
  return final_map;
}
