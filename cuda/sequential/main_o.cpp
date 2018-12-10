#include "../lib/read_csv.cpp"
#include "../lib/helpers.cpp"
#include <string>
using namespace std;


int main () {
  data_map college_data = read_csv("../datasets/College.csv");
  college_dataset data = fill_college_struct(college_data);








  // for (int i=0; i < clusters.size(); i++) {
  //   cout << i << ": ";
  //   print(clusters[i]);
  //   cout << "\n\n\n";
  // }
}
