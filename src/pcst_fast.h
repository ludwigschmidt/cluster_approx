#ifndef __PCST_FAST_H__
#define __PCST_FAST_H__

#include <string>
#include <utility>
#include <vector>

#include "pairing_heap.h"
#include "priority_queue.h"

namespace cluster_approx {

class PCSTFast {
 public:
  enum PruningMethod {
    kNoPruning = 0,
    kSimplePruning,
    kGWPruning,
    kUnknownPruning,
  };

  const static int kNoRoot = -1;

  static PruningMethod parse_pruning_method(const std::string& input);

  PCSTFast(int n_,
           const std::vector<std::pair<int, int> >& edges_,
           const std::vector<double>& prizes_,
           const std::vector<double>& costs_,
           int root_,
           int target_num_active_clusters_,
           PruningMethod pruning_,
           int verbosity_level_,
           void (*output_function_)(const char*));
  
  ~PCSTFast();

  bool run(std::vector<int>* result);
 
 private:
  typedef PairingHeap<double, int> PairingHeapType;
  typedef PriorityQueue<double, int> PriorityQueueType;

  struct EdgeInfo {
    int inactive_merge_event;
    bool good;                // marks whether the edge survives simple pruning
  };

  struct EdgePart {
    // TODO: can remove this property?
    double next_event_time;
    double next_event_val;
    bool deleted;
    PairingHeapType::ItemHandle heap_node;
  };

  struct InactiveMergeEvent {
    int active_cluster_index;
    int inactive_cluster_index;
    int active_cluster_node;
    int inactive_cluster_node;
  };

  struct Cluster {
    PairingHeapType edge_parts;
    double active_start_time;
    double active_end_time;
    double merge_time;
    // TODO: can remove this property?
    double becoming_inactive_time;
    int merged_into;
    double prize_sum;
    double subcluster_moat_sum;
    bool active;
    double moat;
    bool contains_root;
    int skip_up;
    double skip_up_sum;
    int merged_along;
    int child_cluster_1;
    int child_cluster_2;
    bool necessary;

    Cluster(std::vector<PairingHeapType::ItemHandle>* heap_buffer)
        : edge_parts(heap_buffer) {}
  };

  int n;
  const std::vector<std::pair<int, int> >& edges;
  const std::vector<double>& prizes;
  const std::vector<double>& costs;
  int root;
  int target_num_active_clusters;
  PruningMethod pruning;
  int verbosity_level;
  void (*output_function)(const char*);

  std::vector<PairingHeapType::ItemHandle> pairing_heap_buffer;
  std::vector<EdgePart> edge_parts;
  std::vector<EdgeInfo> edge_info;
  std::vector<Cluster> clusters;
  std::vector<InactiveMergeEvent> inactive_merge_events;
  PriorityQueueType clusters_deactivation;
  PriorityQueueType clusters_next_edge_event;
  double current_time;
  double eps;
  std::vector<bool> node_deleted;

  std::vector<std::pair<int, double> > path_compression_visited;
  std::vector<int> cluster_queue;
  std::vector<std::vector<int> > phase3_neighbors;
  
  const static int kOutputBufferSize = 10000;
  char output_buffer[kOutputBufferSize];
  
  void get_next_edge_event(double* next_time,
                           int* next_cluster_index,
                           int* next_edge_part_index);

  void remove_next_edge_event(int next_cluster_index);
  
  void get_next_cluster_event(double* next_time, int* next_cluster_index);

  void remove_next_cluster_event();
  
  void get_sum_on_edge_part(int edge_part_index,
                            double* total_sum,
                            double* finished_moat_sum,
                            int* cur_cluser_index);

  void mark_edges_as_good(int start_cluster_index);

  void mark_clusters_as_necessary(int start_cluster_index);

  void mark_nodes_as_deleted(int start_node_index, int parent_node_index);

  int get_other_edge_part_index(int edge_part_index) {
    if (edge_part_index % 2 == 0) {
      return edge_part_index + 1;
    } else {
      return edge_part_index - 1;
    }
  }
};

}  // namespace cluster_approx


#endif
