#include <string>
#include <utility>
#include <vector>

#include <pybind11/pybind11.h>

#include "pcst_fast.h"

namespace py = pybind11;

void output_function(const char* output) {
  py::print(output, py::arg("flush")=true);
}

void pcst_fast(
    py::array_t<int, py::array::c_style> edges,
    py::array_t<double, py::array::c_style> prizes,
    py::array_t<double, py::array::c_style> costs,
    int root,
    int num_clusters,
    const std::string& pruning,
    int verbosity_level) {
  py::buffer_info edges_info = edges.request();
  if (edges_info.ndim != 2) {
    throw std::invalid_argument("Edges must be a two-dimensional array.");
  }
  if (edges_info.shape[1] != 2) {
    throw std::invalid_argument("The edges array must have two columns.");
  }
  if (edges_info.itemsize != sizeof(int)) {
    throw std::invalid_argument("The edges itemsize does not match "
                                "sizeof(int).");
  }
  int num_edges = edges_info.shape[0];
  int* edges_ptr = edges_info.ptr;
  std::vector<std::pair<int, int> >& tmp_edges(num_edges);
  for (int ii = 0; ii < num_edges; ++ii) {
    tmp_edges[ii].first = edges_ptr[2 * ii];
    tmp_edges[ii].second = edges_ptr[2 * ii + 1];
  }

  py::buffer_info prizes_info = prizes.request();
  if (prizes_info.ndim != 1) {
    throw std::invalid_argument("Prizes must be a one-dimensional array.");
  }
  if (prizes_info.itemsize != sizeof(double)) {
    throw std::invalid_argument("The prizes itemsize does not match "
                                "sizeof(double).");
  }
  int num_nodes = prizes_info.shape[0];
  double* prizes_ptr = prizes_info.ptr;
  std::vector<double> tmp_prizes(num_nodes);
  for (int ii = 0; ii < num_nodes; ++ii) {
    tmp_prizes[ii] = prizes_ptr[ii];
  }
  
  py::buffer_info costs_info = costs.request();
  if (costs_info.ndim != 1) {
    throw std::invalid_argument("Costs must be a one-dimensional array.");
  }
  if (costs_info.itemsize != sizeof(double)) {
    throw std::invalid_argument("The costs itemsize does not match "
                                "sizeof(double).");
  }
  if (costs_info.shape[0] != num_edges) {
    throw std::invalid_argument("The size of the costs array does not match "
                                "the number of rows in the edges array.");
  }
  double* costs_ptr = costs_info.ptr;
  std::vector<double> tmp_costs(num_edges);
  for (int ii = 0; ii < num_edges; ++ii) {
    tmp_costs[ii] = costs_ptr[ii];
  }
  
  PCSTFast::PruningMethod pruning_method =
      PCSTFast::parse_pruning_method(pruning);
  PCSTFast algo(tmp_edges, tmp_prizes, tmp_costs, root, num_clusters,
                pruning_method, verbosity_level, output_function);
  std::vector<int> result_nodes;
  std::vector<int> result_edges;
  algo.run(&result_nodes, &result_edges);

}

PYBIND11_PLUGIN(pcst_fast) {
  py::module m("example", "pybind11 example plugin");

  return m.ptr();
}
