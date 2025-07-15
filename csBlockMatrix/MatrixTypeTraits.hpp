#pragma once
#include <csBlockMatrix/Global.hpp>

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <vector>

#include <csBlockMatrix/DenseMatrixBlock.hpp>
#include <csBlockMatrix/DiagonalMatrixBlock.hpp>
#include <csBlockMatrix/SparseCoeffDiagonalMatrixBlock.hpp>
#include <csBlockMatrix/SparseCoeffMatrixBlock.hpp>
#include <csBlockMatrix/SparseMatrixBlock.hpp>

namespace csBlockMatrix {

template <int matType, class T, int Ordering, int BR, int BC, int NBR = csBlockMatrix::Dynamic, int NBC = csBlockMatrix::Dynamic>
class MatrixBlockIterableTypeTraits {};

template <class T, int Ordering, int BR, int BC, int NBR, int NBC>
class MatrixBlockIterableTypeTraits<csBlockMatrix::BlockDense, T, Ordering, BR, BC, NBR, NBC> {
public:
  using MatrixType = DenseMatrixBlockIterable<T, Ordering, BR, BC, NBR, NBC>;
};

template <class T, int Ordering, int BR, int BC, int NBR, int NBC>
class MatrixBlockIterableTypeTraits<csBlockMatrix::BlockDiagonal, T, Ordering, BR, BC, NBR, NBC> {
public:
  using MatrixType = DiagonalMatrixBlockIterable<T, Ordering, BR, BC, NBR, NBC>;
};

template <class T, int Ordering, int BR, int BC, int NBR, int NBC>
class MatrixBlockIterableTypeTraits<csBlockMatrix::BlockSparse, T, Ordering, BR, BC, NBR, NBC> {
public:
  using MatrixType = SparseMatrixBlock<T, Ordering, BR, BC, NBR, NBC>;
};

template <class T, int Ordering, int BR, int BC, int NBR, int NBC>
class MatrixBlockIterableTypeTraits<csBlockMatrix::BlockCoeffSparse, T, Ordering, BR, BC, NBR, NBC> {
public:
  using MatrixType = SparseCoeffMatrixBlock<T, Ordering, BR, BC, NBR, NBC>;
};

template <class T, int Ordering, int BR, int BC, int NBR, int NBC>
class MatrixBlockIterableTypeTraits<csBlockMatrix::SparseCoeffBlockDiagonal, T, Ordering, BR, BC, NBR, NBC> {
public:
  using MatrixType = SparseCoeffDiagonalMatrixBlock<T, Ordering, BR, BC, NBR, NBC>;
};

} // namespace csBlockMatrix
