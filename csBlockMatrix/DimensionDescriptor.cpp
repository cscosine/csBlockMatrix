#include <csBlockMatrix/DimensionDescriptor.hpp>

namespace csBlockMatrix {

DimensionDescriptorBase<csBlockMatrix::Dynamic>::DimensionDescriptorBase()
    : _nb(0) {}

DimensionDescriptorBase<csBlockMatrix::Dynamic>::DimensionDescriptorBase(int nb)
    : _nb(nb) {}

DimensionDescriptorBase<csBlockMatrix::Dynamic>::~DimensionDescriptorBase() {}

void DimensionDescriptorBase<csBlockMatrix::Dynamic>::setNumBlocks(int nb) {
  this->_nb = nb;
}

} // namespace csBlockMatrix
