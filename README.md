cluster_approx
==============

A library for clustering graph-structured data based on the prize-collecting Steiner tree problem.

Installation
------------

The library has no dependencies besides a basic build system for C++03.
Both g++ and clang are currently supported.

### Matlab

Simply compile the mex wrapper with the supplied makefile:

    make mexfiles

The makefile assumes that Matlab's mex compiler is available via `mex`.
The resulting mex function has the name `cluster_grid_pcst`.


Usage
-----

### Matlab

The basic function signature is

    support, sparsity = cluster_grid_pcst(values, num_clusters, lambda)

The parameters have the following meaning:
* `values`: a 2D double matrix containing the value at each grid node (or equivalently, pixel). Each element must be non-negative.
* `num_clusters`: the target number of clusters in the output. This is a hard constraint which the output will always satisfy.
* `lambda`: the cost of the edges in the grid. This can be used as a Lagrangian relaxation parameter for controling the sparsity of the output: larger lambda means smaller support. Lambda must always be non-negative.

The output variables are:
* `support`: a 2D double matrix indicating the resulting support. Supported elements are marked with a 1.0, other elements with a 0.0.
* `sparsity`: the sparsity of the returned support. This output variable is optional.

Moreover, `cluster_grid_pcst` also accepts a fourth, optional paramter `opts` with the following fields, all of which are optional:
* `verbose`: a double value indicating the level of log messages written to the Matlab shell. Currently, the following values are supported:
    - `0` means no output (unless errors occur).
    - `2` prints a large amount of debug output from the inner PCST algorithm.
* `gamma`: If this double value is present, the underlying PCST problem is solved on a *rooted* graph. This means that the input graph contains an extra root node connected to all grid nodes. The cost of the edges from the root to the grid nodes is `lambda (1 + gamma)`. If the clustering problem should correspond to the relaxed cluster model, `num_clusters` should be set to 0. This has the effect that the number of clusters in the output is not explicitly controlled: after removing the root node, the support on the grid nodes can contain many clusters. `gamma` gives indirect control over the number of clusters in this case: larger gamma means a smaller number of clusters. Note that the overall sparsity and number of clusters depends on both `gamma` and `lambda`, the two variables are not orthogonal.
