# csBlockMatrix

Block-structured matrices in C++17 on Eigen: each dimension described by a `DimensionDescriptor` (blocks of a
compile-time, runtime or variable size; a compile-time or dynamic number of blocks), with dense and sparse storage
layouts. The linear algebra under [csNelson](https://github.com/cscosine/csNelson) (Hessians, Schur complement).

Built and released as part of [csoptimization](https://github.com/cscosine/csoptimization), with
[csCMake](https://github.com/cscosine/csCMake). Early stage: the review of the design and its next steps is
[ASSESSMENT.md](ASSESSMENT.md).
