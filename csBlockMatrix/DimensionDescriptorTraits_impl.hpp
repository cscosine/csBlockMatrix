#pragma once
#include <csBlockMatrix/DimensionDescriptorTraits.hpp>

namespace csBlockMatrix {

template <int B, int NB> constexpr int DimensionDescriptorTraits<B, NB>::numBlocksAtCompileTime;
template <int B, int NB> constexpr int DimensionDescriptorTraits<B, NB>::blockSizeAtCompileTime;
template <int B, int NB> constexpr int DimensionDescriptorTraits<B, NB>::numElementsAtCompileTime;

template <int B> constexpr int DimensionDescriptorTraits<B, csBlockMatrix::Dynamic>::numBlocksAtCompileTime;
template <int B> constexpr int DimensionDescriptorTraits<B, csBlockMatrix::Dynamic>::blockSizeAtCompileTime;
template <int B> constexpr int DimensionDescriptorTraits<B, csBlockMatrix::Dynamic>::numElementsAtCompileTime;

template <int NB> constexpr int DimensionDescriptorTraits<csBlockMatrix::Dynamic, NB>::numBlocksAtCompileTime;
template <int NB> constexpr int DimensionDescriptorTraits<csBlockMatrix::Dynamic, NB>::blockSizeAtCompileTime;
template <int NB> constexpr int DimensionDescriptorTraits<csBlockMatrix::Dynamic, NB>::numElementsAtCompileTime;

template <int NB> constexpr int DimensionDescriptorTraits<csBlockMatrix::Variable, NB>::numBlocksAtCompileTime;
template <int NB> constexpr int DimensionDescriptorTraits<csBlockMatrix::Variable, NB>::blockSizeAtCompileTime;
template <int NB> constexpr int DimensionDescriptorTraits<csBlockMatrix::Variable, NB>::numElementsAtCompileTime;

} // namespace csBlockMatrix
