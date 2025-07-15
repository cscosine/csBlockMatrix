#include <csBlockMatrix/DimensionDescriptorTraits.hpp>

namespace csBlockMatrix {

constexpr int DimensionDescriptorTraits<csBlockMatrix::Dynamic, csBlockMatrix::Dynamic>::numBlocksAtCompileTime;
constexpr int DimensionDescriptorTraits<csBlockMatrix::Dynamic, csBlockMatrix::Dynamic>::blockType;
constexpr int DimensionDescriptorTraits<csBlockMatrix::Dynamic, csBlockMatrix::Dynamic>::blockSizeAtCompileTime;
constexpr int DimensionDescriptorTraits<csBlockMatrix::Dynamic, csBlockMatrix::Dynamic>::numElementsAtCompileTime;

constexpr int DimensionDescriptorTraits<csBlockMatrix::Variable, csBlockMatrix::Dynamic>::numBlocksAtCompileTime;
constexpr int DimensionDescriptorTraits<csBlockMatrix::Variable, csBlockMatrix::Dynamic>::blockType;
constexpr int DimensionDescriptorTraits<csBlockMatrix::Variable, csBlockMatrix::Dynamic>::blockSizeAtCompileTime;
constexpr int DimensionDescriptorTraits<csBlockMatrix::Variable, csBlockMatrix::Dynamic>::numElementsAtCompileTime;

} // namespace csBlockMatrix
