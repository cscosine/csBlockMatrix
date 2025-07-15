#pragma once
#include <csBlockMatrix/MatrixBlockBase.hpp>

#include <csBlockMatrix/MatrixBlockDescriptor_impl.hpp>

namespace csBlockMatrix {

template <int BR, int BC, int NBR, int NBC>
MatrixBlockBase<BR, BC, NBR, NBC>::MatrixBlockBase() {}

template <int BR, int BC, int NBR, int NBC>
MatrixBlockBase<BR, BC, NBR, NBC>::MatrixBlockBase(const BlockDescriptor& blockDesc)
    : _blockDescriptor(blockDesc) {}

template <int BR, int BC, int NBR, int NBC>
MatrixBlockBase<BR, BC, NBR, NBC>::~MatrixBlockBase() {}

template <int BR, int BC, int NBR, int NBC>
void MatrixBlockBase<BR, BC, NBR, NBC>::resize(const BlockDescriptor& blockDesc) {
  _blockDescriptor = blockDesc;
}

} // namespace csBlockMatrix
