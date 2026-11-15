# csBlockMatrix — Assessment (2026-11-16)

Scope: library code review (~3k LOC excluding tests), its use inside csNelson, and a Release run of the csNelson
BA benchmark. Companion document: `csNelson/ASSESSMENT.md`.

## What it is

Block-structured matrices where each dimension (rows / cols) is described by a `DimensionDescriptor<B, NB>`:

- `B > 0` — all blocks have the same compile-time size
- `B = Dynamic` — all blocks have the same runtime size
- `B = Variable` — each block has its own runtime size (`std::vector<int>` + prefix starts)
- `NB` — number of blocks, compile-time or `Dynamic`

Five storage layouts:

| Layout | Storage | Notes |
|---|---|---|
| `BlockDense` | one `Eigen::Matrix` | blocks are `Eigen::Block` views |
| `BlockDiagonal` | `std::vector<Eigen::Matrix>` | |
| `BlockSparse` | `std::vector<Eigen::Matrix>` + CSR/CSC of blocks | one allocation per block if dynamic |
| `BlockCoeffSparse` | `Eigen::SparseMatrix` + per-block `Map` with stride | **zero-copy hand-off to Eigen sparse solvers** |
| `SparseCoeffBlockDiagonal` | `Eigen::SparseMatrix`, diagonal blocks | |

`SparsityPattern<Ordering>` (vector of `std::set<int>`) is the builder; `SparsityPatternBlockDescriptor` maps
blocks to coefficient offsets/strides for the coeff-sparse layouts.

## Strengths

- Compile-time block sizes flow through to fixed-size Eigen types → small dense kernels get fully unrolled/vectorized.
- `BlockCoeffSparse` is a good idea: the Hessian is assembled in place inside the exact storage a sparse
  factorization consumes — no `fillCCS`-style copy as in g2o.
- Uniform `blockUID` addressing (stable integer per non-zero block) is what enables csNelson's precomputed
  multiplication plans and lock-free assembly. This is the most valuable abstraction in the library.
- Choosing the layout per matrix (U, V, W, S) at compile time is a real differentiator vs. g2o/Ceres.

## Issues found

| Where | Issue |
|---|---|
| `SparseMatrixBlock_impl.hpp:20` | Constructor stores a **new empty** sparsity pattern instead of the `sp` passed in; `sparsityPattern()` is wrong after construction (the `resize()` path is fine). |
| `SparseMatrixBlock.hpp` | `_sparsityPattern` member is public. |
| `BlockSparse` / `BlockDiagonal` | Storage is `std::vector<Eigen::Matrix>`: with dynamic block sizes each block is a separate heap allocation; even with fixed sizes it is not a single flat buffer usable by BLAS-batched / GPU kernels. |
| `SparsityPattern` | `std::vector<std::set<int>>` is slow to build for large problems (node allocation per entry). Fine for setup of small/medium problems, will show up at 10⁶+ blocks. Sort+unique on a vector of pairs is usually 5–10× faster. |
| `InnerIterator` | Members declared in a different order than initialized (`-Wreorder`); `virtual` destructor on an iterator is unnecessary overhead. |
| `DimensionDescriptor<Variable>` default ctor | Creates `numBlocks()` empty blocks ("no sense" comment in code). |
| Tests | ~4k LOC of near-duplicate tests per layout; `TEMPLATE_TEST_CASE` would shrink them and compile time. |

## Proposed extension: compile-time periodic block patterns

Example: block sizes `3,4,3,2,3,4,3,2,...` (or VIO states `6,3,6` = pose, velocity, bias, repeated per frame).

Today you must choose between:

- `Variable` — correct, but every block is runtime-sized → `Eigen::Matrix<T, Dynamic, Dynamic>`, no unrolling,
  heap allocation per block, `blockStart` via a lookup table;
- fixed `B = 12` super-blocks — fast, but loses the inner block structure (sparsity between sub-blocks,
  per-sub-block addressing, edges touching only one sub-block).

A **periodic pattern** is a genuinely new category between `Fixed` and `Variable`:

```cpp
template <int... Bs> struct Periodic {
  static constexpr int period = sizeof...(Bs);                 // 4
  static constexpr int superSize = (Bs + ...);                  // 12
  static constexpr std::array<int, period> sizes{Bs...};
  static constexpr std::array<int, period> offsets = /* exclusive prefix sum */;
};
// blockSize(i)  = sizes[i % period]
// blockStart(i) = (i / period) * superSize + offsets[i % period]   // no lookup table
```

What it buys:

- `blockStart`/`blockSize` are arithmetic, no memory traffic.
- Every block has one of `period` compile-time sizes, so kernels can dispatch on the **phase**
  (`i % period`) to fixed-size instantiations: for a matrix that is `period_r × period_c` instantiations
  (16 for 4×4), generated with `std::index_sequence`; at runtime a small switch / function table.
- Storage can stay one flat buffer with computable offsets (good for GPU later).
- Typical problems: VIO / sliding-window (pose+vel+bias), calibration (pose+intrinsics interleaved),
  multi-body systems, spline control points with mixed dimensions.
- Continuous-time SLAM: Gaussian-process motion priors produce block-tridiagonal matrices, and B-splines of
  order k produce banded matrices (each residual touches k consecutive control points). Both have constant,
  repeating structure that suits compile-time patterns (see `csNelson/ASSESSMENT.md`, "csNelson as a general
  SLAM and mapping solver").

Design implications:

1. The current API encodes `B` as an `int` (`Fixed=0`, `Dynamic=-1`, `Variable=-2`). A periodic pattern doesn't fit
   in an `int`; the clean path is to move to **type-based** descriptors: `Fixed<6>`, `Dynamic`, `Variable`,
   `Periodic<3,4,3,2>`. This is an API break — best done together with the other refactors below.
2. A block accessor can no longer return a single fixed type. Options: a visitor
   (`forEachBlock(fn)` where `fn` is generic and gets the fixed-size block), or phase-templated access
   (`block<PhaseR, PhaseC>(r, c)`). The visitor fits naturally with a `parallel_for` backend.
3. Overlap with csNelson "sections": separate variable groups (N sections) allow Schur elimination per group;
   a periodic pattern keeps variables **interleaved** in one ordering (good for banded/temporal structure).
   Both are useful; they are complementary, not alternatives.

## Roadmap (csBlockMatrix part)

1. Fix the constructor bug; add tests that check `sparsityPattern()` after construction.
2. Flat storage for `BlockSparse`/`BlockDiagonal`: one contiguous buffer + per-block offsets (BSR-like).
   Required for CPU cache behaviour and for any GPU backend.
3. Faster `SparsityPattern` builder (vector of pairs, sort+unique, then compress).
4. Type-based dimension descriptors, including `Periodic<...>`.
5. Visitor / `parallel_for`-style block iteration API that a CUDA backend can implement.
6. Collapse duplicated tests with templated test cases.
7. Export paths instead of competing with solvers: compressed-column (CHOLMOD / Eigen), compressed-row (cuDSS),
   BaSpaCho block layout, and BSR (cuSPARSE / MKL) when blocks happen to be uniform.

## Related work

Compiled from memory on 2026-11-16; check each project's current state before depending on it.

### Closest in spirit (block matrices built for optimization)

| Library | What it offers | Compared with csBlockMatrix |
|---|---|---|
| **SLAM++** (Brno University of Technology; Polok, Ila et al.) | Its block matrix type, `CUberBlockMatrix`, stores blocks of varying sizes. Its fast operations take a compile-time list of the block sizes that can occur, and at runtime pick a fixed-size Eigen version for each block. | **Closest relative, and the nearest thing to the `Periodic<...>` idea.** Study how it chooses kernels at runtime and keeps compile times under control. Development activity is low. |
| **g2o** `SparseBlockMatrix` (+ `...CCS`, `...HashMap`, `...Diagonal`) | Blocks of varying sizes stored column by column, using runtime size arrays. | Direct ancestor. No compile-time sizes per dimension; copies data out (`fillCCS`) before solving; Schur step takes locks. |
| **Ceres** (internal) `BlockSparseMatrix`, `CompressedRowBlockStructure`, `PartitionedMatrixView<kRow,kE,kF>`, `BlockRandomAccess*Matrix` | Very optimized, with block sizes fixed at compile time for common bundle-adjustment shapes; CUDA versions in Ceres 2.2. | Internal API, not usable as a library. Compile-time specialization only covers a fixed set of sizes, not arbitrary patterns. |
| **GTSAM** `SymmetricBlockMatrix`, `VerticalBlockMatrix` | A dense matrix divided into blocks, one per variable, with sizes known at runtime. | Dense only; used inside factors and elimination, not for global sparse storage. |
| **Eigen unsupported** `BlockSparseMatrix` | One block size for the whole matrix (compile-time or runtime), or varying sizes at runtime. | Experimental and barely maintained, but the natural "standard" integration point. |

### General sparse libraries with a block format (usually one block size for the whole matrix)

| Library | Notes |
|---|---|
| **Intel MKL** BSR | Square blocks of one size, set at runtime. Fast matrix–vector products on CPU. |
| **cuSPARSE** BSR (plus the generic BSR API in recent CUDA 12) | One block size, on GPU. Candidate backend for matrix–vector products in PCG. |
| **Kokkos Kernels** `BsrMatrix` | One block size; portable across CPU, CUDA and HIP. Also a possible model for a `parallel_for` backend helper. |
| **PETSc** BAIJ / SBAIJ, **Trilinos** `Tpetra::BlockCrsMatrix` | One block size, aimed at large solvers (finite elements, CFD). Too heavy for this use case. |
| **Blaze** | A sparse or dense matrix whose elements are themselves small fixed-size matrices, e.g. `CompressedMatrix<StaticMatrix<double,3,3>>`. Elegant for uniform block sizes, but can't handle varying sizes. |

### Solvers that work on blocks

| Library | Notes |
|---|---|
| **BaSpaCho** (Meta) | Supernodal Cholesky working directly on variable-size parameter blocks, CPU and CUDA. Best fit as an S solver; could take csBlockMatrix output almost directly. |
| **CHOLMOD** (SuiteSparse, supernodal) | Detects dense blocks internally, but its interface works on individual coefficients. |
| **cuDSS** (NVIDIA) | Direct sparse solver on GPU, taking compressed-row (CSR) input. |

### Helpers for the small dense kernels

**libxsmm** (CPU, JIT-compiled small matrix products), **MAGMA batched** and **cuBLAS batched** (GPU),
**CUTLASS**: useful as kernels underneath, not as storage.

### Positioning

Each piece exists somewhere: block sizes fixed at compile time (Ceres, SLAM++), blocks of varying size (g2o,
SLAM++, Eigen unsupported), GPU block storage with one block size (cuSPARSE, Kokkos). No reusable library is
known to combine:

- a block-size scheme chosen separately for each dimension (fixed / dynamic / variable / periodic);
- the storage layout chosen per matrix;
- zero-copy hand-off to a sparse solver;
- stable block UIDs enabling precomputed products and lock-free assembly.

That combination is csBlockMatrix's differentiator. Interoperate with the solvers above rather than replacing them.
