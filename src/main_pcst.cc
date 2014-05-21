#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>

#include "pcst_fast.h"

using cluster_approx::PCSTFast;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::vector;

void output_function(const char* s) {
  fprintf(stderr, "%s", s);
  fflush(stderr);
}

int main() {
  /*int n = 3;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  vector<double> prizes;
  prizes.push_back(0);
  prizes.push_back(5);
  prizes.push_back(6);
  vector<double> costs;
  costs.push_back(3);
  costs.push_back(4);
  int root = 0;
  int target_num_active_clusters = 0;
  PCSTFast::PruningMethod pruning = PCSTFast::kNoPruning;
  int verbosity_level = 2;*/

  int n = 4;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  edges.push_back(make_pair(2, 3));
  vector<double> prizes;
  prizes.push_back(10);
  prizes.push_back(0);
  prizes.push_back(1);
  prizes.push_back(10);
  vector<double> costs;
  costs.push_back(10);
  costs.push_back(4);
  costs.push_back(3);
  int root = 0;
  int target_num_active_clusters = 0;
  PCSTFast::PruningMethod pruning = PCSTFast::kNoPruning;
  int verbosity_level = 2;

  PCSTFast algo(n, edges, prizes, costs, root, target_num_active_clusters,
                pruning, verbosity_level, output_function);
  
  vector<int> result;
  algo.run(&result);

  cout << "Result: " << endl;
  for (size_t ii = 0; ii < result.size(); ++ii) {
    cout << result[ii] << " ";
  }
  cout << endl;

  return 0;
}
