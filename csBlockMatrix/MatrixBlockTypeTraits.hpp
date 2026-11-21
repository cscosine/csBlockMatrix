#pragma once
#include <csBlockMatrix/Global.hpp>

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <vector>

#include <csBlockMatrix/DimensionDescriptor.hpp>
#include <csBlockMatrix/MatrixBlockDescriptor.hpp>

namespace csBlockMatrix {

template <class T, int BR, int NBR = csBlockMatrix::Dynamic> class VectorBlockTraits {
public:
  using BlockTraits = typename DimensionDescriptor<BR, NBR>::Traits;

  using StorageType = Eigen::Matrix<T, BlockTraits::numElementsAtCompileTime, 1>;
  using SubVectorType = Eigen::Matrix<T, BlockTraits::blockSizeAtCompileTime, 1>;
  using SegmentType = typename Eigen::Block<StorageType, BlockTraits::blockSizeAtCompileTime, 1>;
  using ConstSegmentType = typename Eigen::Block<const StorageType, BlockTraits::blockSizeAtCompileTime, 1>;
};

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

template <int matType, class T, int BR, int BC, int NBR = csBlockMatrix::Dynamic, int NBC = csBlockMatrix::Dynamic>
class MatrixBlockTypeTraits {};

//--------------------------------------------------------------------------------------------------------------------------

template <class T, int BR, int BC, int NBR, int NBC>
class MatrixBlockTypeTraits<csBlockMatrix::BlockDense, T, BR, BC, NBR, NBC> {
public:
  using RowTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorRow::Traits;
  using ColTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorCol::Traits;

  using StorageType = Eigen::Matrix<T, RowTraits::numElementsAtCompileTime, ColTraits::numElementsAtCompileTime>;
  using SubMatrixType = Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>;
  using BlockType =
      typename Eigen::Block<StorageType, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>;
  using ConstBlockType =
      typename Eigen::Block<const StorageType, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>;
};

//----------------------------------------------------------------------------------------------------

template <class T, int BR, int BC, int NBR, int NBC>
class MatrixBlockTypeTraits<csBlockMatrix::BlockDiagonal, T, BR, BC, NBR, NBC> {
public:
  using RowTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorRow::Traits;
  using ColTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorCol::Traits;

  using StorageType =
      std::vector<Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>>;
  using SubMatrixType = Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>;
  using BlockType = SubMatrixType &;
  using ConstBlockType = const SubMatrixType &;
};

//----------------------------------------------------------------------------------------------------

template <class T, int BR, int BC, int NBR, int NBC>
class MatrixBlockTypeTraits<csBlockMatrix::BlockSparse, T, BR, BC, NBR, NBC> {
public:
  using RowTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorRow::Traits;
  using ColTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorCol::Traits;

  using StorageType =
      std::vector<Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>>;
  using SubMatrixType = Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>;
  using BlockType = SubMatrixType &;
  using ConstBlockType = const SubMatrixType &;
};

//----------------------------------------------------------------------------------------------------

template <class T, int BR, int BC, int NBR, int NBC>
class MatrixBlockTypeTraits<csBlockMatrix::BlockCoeffSparse, T, BR, BC, NBR, NBC> {
public:
  using RowTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorRow::Traits;
  using ColTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorCol::Traits;

  template <int Ordering> using StorageType = Eigen::SparseMatrix<T, Ordering>;
  using SubMatrixType = Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>;

  template <int Ordering>
  using BlockType =
      Eigen::Map<Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime, Ordering>, 0,
                 Eigen::OuterStride<>>;

  template <int Ordering>
  using ConstBlockType =
      Eigen::Map<const Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime, Ordering>,
                 0, Eigen::OuterStride<>>;
};

//----------------------------------------------------------------------------------------------------

template <class T, int BR, int BC, int NBR, int NBC>
class MatrixBlockTypeTraits<csBlockMatrix::SparseCoeffBlockDiagonal, T, BR, BC, NBR, NBC> {
public:
  using RowTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorRow::Traits;
  using ColTraits = typename MatrixBlockDescriptor<BR, BC, NBR, NBC>::DimensionDescriptorCol::Traits;

  template <int Ordering> using StorageType = Eigen::SparseMatrix<T, Ordering>;
  using SubMatrixType = Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime>;

  template <int Ordering>
  using BlockType =
      Eigen::Map<Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime, Ordering>>;

  template <int Ordering>
  using ConstBlockType = Eigen::Map<
      const Eigen::Matrix<T, RowTraits::blockSizeAtCompileTime, ColTraits::blockSizeAtCompileTime, Ordering>>;
};

} // namespace csBlockMatrix
