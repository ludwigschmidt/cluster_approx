#ifndef __PCST_FAST_SWIG_H__
#define __PCST_FAST_SWIG_H__

#include <cstdio>
#include <string>
#include <vector>

#include "pcst_fast.h"

namespace cluster_approx {

void output_function(const char* output) {
  printf(output);
  fflush(stdout);
}

std::vector<int> pcst_fast(int n,
                           const std::vector<std::pair<int, int> >& edges,
                           const std::vector<double>& prizes,
                           const std::vector<double>& costs,
                           int root,
                           int num_clusters,
                           const std::string& pruning,
                           int verbosity_level) {
  PCSTFast::PruningMethod pruning_method =
      PCSTFast::parse_pruning_method(pruning);
  PCSTFast algo(n, edges, prizes, costs, root, num_clusters,
                pruning_method, verbosity_level, output_function);
  std::vector<int> result;
  algo.run(&result);
  return result;
}

}  // namespace cluster_approx

#endif
