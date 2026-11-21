#pragma once
#include <csBlockMatrix/Global.hpp>
#include <vector>

namespace csBlockMatrix {

template <int B, int NB> struct DimensionDescriptorTraits {
  using BlockSizeType = int;
  using BlockSizeTypePar = int;

  static constexpr int numBlocksAtCompileTime = NB;
  static constexpr int blockSizeAtCompileTime = B;
  static constexpr int blockType = csBlockMatrix::Fixed;
  static constexpr int numElementsAtCompileTime = NB * B;
};

template <int B> struct DimensionDescriptorTraits<B, csBlockMatrix::Dynamic> {
  using BlockSizeType = int;
  using BlockSizeTypePar = int;

  static constexpr int numBlocksAtCompileTime = csBlockMatrix::Dynamic;
  static constexpr int blockSizeAtCompileTime = B;
  static constexpr int blockType = csBlockMatrix::Fixed;
  static constexpr int numElementsAtCompileTime = csBlockMatrix::Dynamic;
};

template <int NB> struct DimensionDescriptorTraits<csBlockMatrix::Dynamic, NB> {
  using BlockSizeType = int;
  using BlockSizeTypePar = int;

  static constexpr int numBlocksAtCompileTime = NB;
  static constexpr int blockSizeAtCompileTime = csBlockMatrix::Dynamic;
  static constexpr int blockType = csBlockMatrix::Dynamic;
  static constexpr int numElementsAtCompileTime = csBlockMatrix::Dynamic;
};

template <> struct DimensionDescriptorTraits<csBlockMatrix::Dynamic, csBlockMatrix::Dynamic> {
  using BlockSizeType = int;
  using BlockSizeTypePar = int;

  static constexpr int numBlocksAtCompileTime = csBlockMatrix::Dynamic;
  static constexpr int blockSizeAtCompileTime = csBlockMatrix::Dynamic;
  static constexpr int blockType = csBlockMatrix::Dynamic;
  static constexpr int numElementsAtCompileTime = csBlockMatrix::Dynamic;
};

template <int NB> struct DimensionDescriptorTraits<csBlockMatrix::Variable, NB> {
  using BlockSizeType = std::vector<int>;
  using BlockSizeTypePar = const std::vector<int> &;

  static constexpr int numBlocksAtCompileTime = NB;
  static constexpr int blockSizeAtCompileTime = csBlockMatrix::Dynamic;
  static constexpr int blockType = csBlockMatrix::Variable;
  static constexpr int numElementsAtCompileTime = csBlockMatrix::Dynamic;
};

template <> struct DimensionDescriptorTraits<csBlockMatrix::Variable, csBlockMatrix::Dynamic> {
  using BlockSizeType = std::vector<int>;
  using BlockSizeTypePar = const std::vector<int> &;

  static constexpr int numBlocksAtCompileTime = csBlockMatrix::Dynamic;
  static constexpr int blockSizeAtCompileTime = csBlockMatrix::Dynamic;
  static constexpr int blockType = csBlockMatrix::Variable;
  static constexpr int numElementsAtCompileTime = csBlockMatrix::Dynamic;
};

} // namespace csBlockMatrix
