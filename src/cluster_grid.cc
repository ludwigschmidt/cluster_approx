#include "cluster_grid.h"

#include <utility>
#include <vector>

#include "pcst_fast.h"

using std::make_pair;
using std::vector;

namespace cluster_approx {

void build_grid_graph(const Matrix2d& values,
                      bool include_root,
                      double gamma,
                      int* n,
                      vector<EdgePair>* edges,
                      vector<double>* prizes,
                      vector<double>* costs,
                      int* root) {
  edges->clear();
  prizes->clear();
  costs->clear();
  *root = PCSTFast::kNoRoot;
  *n = 0;
  int height = static_cast<int>(values.size());
  int width;
  if (height == 0) {
    width = 0;
    return;
  } else {
    width = static_cast<int>(values[0].size());
  }
  *n = width * height;
  if (include_root) {
    *n += 1;
  }

  prizes->resize(*n);

  for (int yy = 0; yy < height; ++yy) {
    for (int xx = 0; xx < width; ++xx) {
      int cur_index = yy * width + xx;
      (*prizes)[cur_index] = values[yy][xx];

      if (xx != width - 1) {
        int next_right = cur_index + 1;
        edges->push_back(make_pair(cur_index, next_right));
        costs->push_back(1.0);
      }

      if (yy != height - 1) {
        int next_down = cur_index + width;
        edges->push_back(make_pair(cur_index, next_down));
        costs->push_back(1.0);
      }
    }
  }

  if (include_root) {
    *root = *n - 1;
    (*prizes)[*root] = 0.0;
    double root_edge_cost = 1.0 + gamma;

    for (int ii = 0; ii < *root; ++ii) {
      edges->push_back(make_pair(*root, ii));
      costs->push_back(root_edge_cost);
    }
  }
}


void convert_forest_to_support(const vector<int>& forest_node_indices,
                               int root,
                               int width,
                               int height,
                               Matrix2b* result) {
  result->clear();
  result->resize(height);
  for (int yy = 0; yy < height; ++yy) {
    (*result)[yy].resize(width, false);
  }

  for (size_t ii = 0; ii < forest_node_indices.size(); ++ii) {
    int node = forest_node_indices[ii];
    if (node != root) {
      int node_xx = node % width;
      int node_yy = node / width;
      (*result)[node_xx][node_yy] = true;
    }
  }
}


bool cluster_grid_pcst(const Matrix2d& values,
                       int target_num_clusters,
                       double lambda,
                       bool include_root,
                       double gamma,
                       PCSTFast::PruningMethod pruning,
                       int verbosity_level,
                       void (*output_function)(const char*),
                       Matrix2b* result,
                       int* result_sparsity) {
  int n;
  vector<EdgePair> edges;
  vector<double> prizes;
  vector<double> costs;
  int root;

  build_grid_graph(values, include_root, gamma, &n, &edges, &prizes, &costs,
                   &root);

  if (n == 0) {
    return false;
  }

  for (size_t ii = 0; ii < costs.size(); ++ii) {
    costs[ii] *= lambda;
  }

  PCSTFast algo(n, edges, prizes, costs, root, target_num_clusters, pruning,
                verbosity_level, output_function);

  vector<int> forest_node_indices;
  vector<int> forest_edge_indices;
  bool res = algo.run(&forest_node_indices, &forest_edge_indices);
  if (!res) {
    return false;
  }
  
  convert_forest_to_support(forest_node_indices, root, values[0].size(),
                            values.size(), result);
  
  *result_sparsity = static_cast<int>(forest_node_indices.size());
  if (include_root) {
    *result_sparsity -= 1;
  }

  return true;
}

}  // namespace cluster_approx
