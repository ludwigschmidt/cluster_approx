#include <cstdio>
#include <utility>
#include <vector>

#include "cluster_grid.h"
#include "pcst_fast.h"

using cluster_approx::cluster_grid_pcst;
using cluster_approx::PCSTFast;
using std::vector;

void output_function(const char* output) {
  printf(output);
}


int main() {
  vector<vector<double> > values;
  values.resize(5);
  values[0].push_back(1.0);
  values[0].push_back(1.0);
  values[0].push_back(0.0);
  values[0].push_back(0.0);
  values[0].push_back(0.0);

  values[1].push_back(1.0);
  values[1].push_back(1.0);
  values[1].push_back(0.0);
  values[1].push_back(0.0);
  values[1].push_back(0.0);

  values[2].push_back(0.0);
  values[2].push_back(0.0);
  values[2].push_back(0.0);
  values[2].push_back(0.0);
  values[2].push_back(0.0);

  values[3].push_back(0.0);
  values[3].push_back(0.0);
  values[3].push_back(0.0);
  values[3].push_back(0.0);
  values[3].push_back(0.0);
  
  values[4].push_back(0.0);
  values[4].push_back(0.0);
  values[4].push_back(1.0);
  values[4].push_back(1.0);
  values[4].push_back(1.0);

  int target_num_clusters = 2;
  double lambda = 0.1;
  bool include_root = false;
  double gamma = -1.0;
  int verbosity_level = 2;

  vector<vector<bool> > result;
  int result_sparsity;

  bool res = cluster_grid_pcst(values, target_num_clusters, lambda,
                               include_root, gamma, PCSTFast::kStrongPruning,
                               verbosity_level, output_function, &result,
                               &result_sparsity);
  
  if (!res) {
    printf("Error returned by cluster_grid_pcst\n");
    return 0;
  }

  printf("Result (sparsity %d):\n", result_sparsity);
  for (int yy = 0; yy < static_cast<int>(result.size()); ++yy) {
    for (int xx = 0; xx < static_cast<int>(result[0].size()); ++xx) {
      printf("%d", static_cast<int>(result[yy][xx]));
    }
    printf("\n");
  }
  
  return 0;
}
