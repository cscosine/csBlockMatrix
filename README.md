# csBlockMatrix

Block-structured dense and sparse matrices in C++ on Eigen

<!-- cs-template:begin (this block is owned by cs-project-template: do not edit; the rest of the file is the project's) -->
## Getting started

csBlockMatrix is built by the project that consumes it: no setup or build CI of its own. Formatting and checks:
`pre-commit install` once, then `pre-commit run --all-files` (also run by `.github/workflows/checks.yml`).

The shared files (setup, checks, CI, agent rules) come from
[cs-project-template](https://github.com/cscosine/cs-project-template); to bring them to its latest version, on a
branch: `copier update --defaults`.
<!-- cs-template:end -->

Block-structured matrices in C++17 on Eigen: each dimension described by a `DimensionDescriptor` (blocks of a
compile-time, runtime or variable size; a compile-time or dynamic number of blocks), with dense and sparse storage
layouts. The linear algebra under [csNelson](https://github.com/cscosine/csNelson) (Hessians, Schur complement).

Built and released as part of [csoptimization](https://github.com/cscosine/csoptimization), with
[csCMake](https://github.com/cscosine/csCMake). Early stage: the review of the design and its next steps is
[ASSESSMENT.md](ASSESSMENT.md).
