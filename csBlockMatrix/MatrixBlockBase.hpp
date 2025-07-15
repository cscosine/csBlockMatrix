#pragma once
#include <csBlockMatrix/Global.hpp>

#include <Eigen/Core>
#include <vector>

#include <csBlockMatrix/MatrixBlockDescriptor.hpp>
#include <csBlockMatrix/MatrixBlockTypeTraits.hpp>

namespace csBlockMatrix {

template <int BR, int BC, int NBR = csBlockMatrix::Dynamic, int NBC = csBlockMatrix::Dynamic>
class MatrixBlockBase {
public:
  using BlockDescriptor = MatrixBlockDescriptor<BR, BC, NBR, NBC>;
  using DimensionDescriptorRow = typename BlockDescriptor::DimensionDescriptorRow;
  using DimensionDescriptorCol = typename BlockDescriptor::DimensionDescriptorCol;

  using RowTraits = typename DimensionDescriptorRow::Traits;
  using ColTraits = typename DimensionDescriptorCol::Traits;

private:
  BlockDescriptor _blockDescriptor;

public:
  // empty
  MatrixBlockBase();
  // with block
  MatrixBlockBase(const BlockDescriptor& blockDesc);

  virtual ~MatrixBlockBase();

  inline int numBlocksRow() const {
    return _blockDescriptor.numBlocksRow();
  }
  inline int numBlocksCol() const {
    return _blockDescriptor.numBlocksCol();
  }

  inline int rowBlockSize(int i) const {
    return _blockDescriptor.rowBlockSize(i);
  }
  inline int colBlockSize(int j) const {
    return _blockDescriptor.colBlockSize(j);
  }

  inline int rowBlockStart(int i) const {
    return _blockDescriptor.rowBlockStart(i);
  }
  inline int colBlockStart(int j) const {
    return _blockDescriptor.colBlockStart(j);
  }

  inline int numElementRows() const {
    return _blockDescriptor.numElementRows();
  }
  inline int numElementCols() const {
    return _blockDescriptor.numElementCols();
  }

  const BlockDescriptor& blockDescriptor() const {
    return _blockDescriptor;
  }

  virtual void resize(const BlockDescriptor& blockDesc);
};

} // namespace csBlockMatrix
