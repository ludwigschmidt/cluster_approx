#include "pcst_fast.h"

#include <algorithm>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "test_helpers.h"

using cluster_approx::PCSTFast;
using std::make_pair;
using std::pair;
using std::vector;

const int kVerbosityLevel = 0;


void RunAlgo(int n,
             const vector<pair<int, int> >& edges,
             const vector<double>& prizes,
             const vector<double>& costs,
             int root,
             int target_num_active_clusters,
             PCSTFast::PruningMethod pruning,
             const vector<int>& expected_node_result,
             const vector<int>& expected_edge_result) {

  vector<int> node_result;
  vector<int> edge_result;
  PCSTFast algo(n, edges, prizes, costs, root, target_num_active_clusters,
                pruning, kVerbosityLevel, WriteToStderr);
  ASSERT_TRUE(algo.run(&node_result, &edge_result));

  std::sort(node_result.begin(), node_result.end());
  std::sort(edge_result.begin(), edge_result.end());
  vector<int> sorted_expected_node_result(expected_node_result);
  std::sort(sorted_expected_node_result.begin(),
            sorted_expected_node_result.end());
  vector<int> sorted_expected_edge_result(expected_edge_result);
  std::sort(sorted_expected_edge_result.begin(),
            sorted_expected_edge_result.end());

  CheckResult(sorted_expected_node_result, node_result);
  CheckResult(sorted_expected_edge_result, edge_result);
}


template <size_t N1, size_t N2, size_t N3, size_t N4>
void RunAlgo(int n,
             const vector<pair<int, int> >& edges,
             const double (&prizes)[N1],
             const double (&costs)[N2],
             int root,
             int target_num_active_clusters,
             PCSTFast::PruningMethod pruning,
             const int (&expected_node_result)[N3],
             const int (&expected_edge_result)[N4]) {
  vector<double> prizes_(begin(prizes), end(prizes));
  vector<double> costs_(begin(costs), end(costs));
  vector<int> expected_node_result_(begin(expected_node_result),
                                    end(expected_node_result));
  vector<int> expected_edge_result_(begin(expected_edge_result),
                                    end(expected_edge_result));
  RunAlgo(n, edges, prizes_, costs_, root, target_num_active_clusters, pruning,
          expected_node_result_, expected_edge_result_);
}


template <size_t N1, size_t N2, size_t N3>
void RunAlgo(int n,
             const vector<pair<int, int> >& edges,
             const double (&prizes)[N1],
             const double (&costs)[N2],
             int root,
             int target_num_active_clusters,
             PCSTFast::PruningMethod pruning,
             const int (&expected_node_result)[N3]) {
  vector<double> prizes_(begin(prizes), end(prizes));
  vector<double> costs_(begin(costs), end(costs));
  vector<int> expected_node_result_(begin(expected_node_result),
                                    end(expected_node_result));
  vector<int> expected_edge_result_(0);
  RunAlgo(n, edges, prizes_, costs_, root, target_num_active_clusters, pruning,
          expected_node_result_, expected_edge_result_);
}


TEST(PCSTFastTest, SimpleTestRootedNoPruning) {
  int n = 3;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  const double prizes[] = {0, 5, 6};
  const double costs[] = {3, 4};
  int root = 0;
  int target_num_active_clusters = 0;
  PCSTFast::PruningMethod pruning = PCSTFast::kNoPruning;

  const int node_result[] = {0, 1, 2};
  const int edge_result[] = {0, 1};

  RunAlgo(n, edges, prizes, costs, root, target_num_active_clusters, pruning,
          node_result, edge_result);
}


TEST(PCSTFastTest, SimpleTestUnrootedNoPruning) {
  int n = 3;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  const double prizes[] = {0, 5, 6};
  const double costs[] = {3, 4};
  int root = -1;
  int target_num_active_clusters = 1;
  PCSTFast::PruningMethod pruning = PCSTFast::kNoPruning;

  const int node_result[] = {1, 2};
  const int edge_result[] = {1};

  RunAlgo(n, edges, prizes, costs, root, target_num_active_clusters, pruning,
          node_result, edge_result);
}


TEST(PCSTFastTest, SimpleTestUnrootedGWPruning) {
  int n = 3;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  const double prizes[] = {0, 5, 6};
  const double costs[] = {3, 4};
  int root = -1;
  int target_num_active_clusters = 1;
  PCSTFast::PruningMethod pruning = PCSTFast::kGWPruning;

  const int node_result[] = {1, 2};
  const int edge_result[] = {1};

  RunAlgo(n, edges, prizes, costs, root, target_num_active_clusters, pruning,
          node_result, edge_result);
}


TEST(PCSTFastTest, SimpleTestUnrootedStrongPruning) {
  int n = 3;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  const double prizes[] = {0, 5, 6};
  const double costs[] = {3, 4};
  int root = -1;
  int target_num_active_clusters = 1;
  PCSTFast::PruningMethod pruning = PCSTFast::kStrongPruning;

  const int node_result[] = {1, 2};
  const int edge_result[] = {1};

  RunAlgo(n, edges, prizes, costs, root, target_num_active_clusters, pruning,
          node_result, edge_result);
}


TEST(PCSTFastTest, Simple2TestRootedNoPruning) {
  int n = 4;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  edges.push_back(make_pair(2, 3));
  const double prizes[] = {10, 0, 1, 10};
  const double costs[] = {10, 4, 3};
  int root = 0;
  int target_num_active_clusters = 0;
  PCSTFast::PruningMethod pruning = PCSTFast::kNoPruning;

  const int node_result[] = {0, 1, 2, 3};
  const int edge_result[] = {1, 2};

  RunAlgo(n, edges, prizes, costs, root, target_num_active_clusters, pruning,
          node_result, edge_result);
}


TEST(PCSTFastTest, Simple2TestRootedGWPruning) {
  int n = 4;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  edges.push_back(make_pair(2, 3));
  const double prizes[] = {10, 0, 1, 10};
  const double costs[] = {10, 4, 3};
  int root = 0;
  int target_num_active_clusters = 0;
  PCSTFast::PruningMethod pruning = PCSTFast::kGWPruning;

  const int node_result[] = {0};
  // edge result should be empty

  RunAlgo(n, edges, prizes, costs, root, target_num_active_clusters, pruning,
          node_result);
}


TEST(PCSTFastTest, Simple3TestRootedNoPruning) {
  int n = 4;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  edges.push_back(make_pair(2, 3));
  const double prizes[] = {10, 10, 1, 10};
  const double costs[] = {10, 6, 5};
  int root = 0;
  int target_num_active_clusters = 0;
  PCSTFast::PruningMethod pruning = PCSTFast::kNoPruning;

  const int node_result[] = {0, 1, 2, 3};
  const int edge_result[] = {0, 1, 2};

  RunAlgo(n, edges, prizes, costs, root, target_num_active_clusters, pruning,
          node_result, edge_result);
}


TEST(PCSTFastTest, Simple3TestRootedGWPruning) {
  int n = 4;
  vector<pair<int, int> > edges;
  edges.push_back(make_pair(0, 1));
  edges.push_back(make_pair(1, 2));
  edges.push_back(make_pair(2, 3));
  const double prizes[] = {10, 10, 1, 10};
  const double costs[] = {10, 6, 5};
  int root = 0;
  int target_num_active_clusters = 0;
  PCSTFast::PruningMethod pruning = PCSTFast::kGWPruning;

  const int node_result[] = {0, 1, 2, 3};
  const int edge_result[] = {0, 1, 2};

  RunAlgo(n, edges, prizes, costs, root, target_num_active_clusters, pruning,
          node_result, edge_result);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

