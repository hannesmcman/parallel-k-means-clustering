#ifndef _READ_CSV
#define _READ_CSV

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

college_dataset fill_college_struct(const data_map& college_data){

    vector<float> sample_map_data = college_data.begin()->second; 
  
    college_dataset data;
    data.size = college_data.size();
    data.dimensions = sample_map_data.size();
    data.names = new string[data.size];
    data.features = new float *[data.size];
    for (int i=0; i<data.size; i++)
      data.features[i] = new float[data.dimensions];
  
    int index = 0;
    for (data_map::const_iterator it = college_data.begin(); it != college_data.end(); it++) {
        data.names[index] = it->first;
        for (int j=0; j<data.dimensions; j++)
          data.features[index][j] = (it->second)[j];
        index++;
    } 
    return data;
}

college_dataset read_csv_to_college_struct(string filename) {
  data_map data = read_csv(filename);
  return fill_college_struct(data);
}

#endif