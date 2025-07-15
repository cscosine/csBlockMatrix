#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <csBlockMatrix/DimensionDescriptor_impl.hpp>

#include <memory>
#include <type_traits>

TEMPLATE_TEST_CASE_SIG("DimensionDescriptor<3|Dynamic>", "[DimensionDescriptor]", ((int B, int NB), B, NB), (3, 5),
                       (3, csBlockMatrix::Dynamic), (csBlockMatrix::Dynamic, 5), (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic)) {

  std::unique_ptr<csBlockMatrix::DimensionDescriptor<B, NB>> d;
  SECTION("Default ctor") {

    d.reset(new csBlockMatrix::DimensionDescriptor<B, NB>());
    REQUIRE(csBlockMatrix::DimensionDescriptor<B, NB>::Traits::numBlocksAtCompileTime == NB);

    d->resize(3, 5);
    REQUIRE(d->numBlocks() == 5);
  }
  SECTION("Block-size ctor") {

    d.reset(new csBlockMatrix::DimensionDescriptor<B, NB>(3));
    REQUIRE(csBlockMatrix::DimensionDescriptor<B, NB>::Traits::numBlocksAtCompileTime == NB);

    d->resize(3, 5);
    REQUIRE(d->numBlocks() == 5);
  }
  SECTION("Block-size + block num ctor") {

    d.reset(new csBlockMatrix::DimensionDescriptor<B, NB>(3, 5));
    REQUIRE(csBlockMatrix::DimensionDescriptor<B, NB>::Traits::numBlocksAtCompileTime == NB);

    REQUIRE(d->numBlocks() == 5);
  }

  int s = 0;
  for (int i = 0; i < d->numBlocks(); i++) {
    REQUIRE(d->blockSize(i) == 3);
    REQUIRE(d->blockStart(i) == 3 * i);
    s += d->blockSize(i);
  }

  REQUIRE(d->numElements() == s);

  if (csBlockMatrix::DimensionDescriptor<B, NB>::Traits::numBlocksAtCompileTime == csBlockMatrix::Dynamic) {
    int b = (csBlockMatrix::DimensionDescriptor<B, NB>::Traits::blockSizeAtCompileTime == csBlockMatrix::Dynamic
                 ? 3
                 : csBlockMatrix::DimensionDescriptor<B, NB>::Traits::blockSizeAtCompileTime);
    d->addBlock(b);

    s = 0;
    for (int i = 0; i < d->numBlocks(); i++) {
      REQUIRE(d->blockSize(i) == 3);
      REQUIRE(d->blockStart(i) == s);
      s += d->blockSize(i);
    }

    REQUIRE(d->numElements() == s);
  }
}

TEMPLATE_TEST_CASE_SIG("DimensionDescriptor<csBlockMatrix::Variable|5>", "[DimensionDescriptor]", (int NB, NB), 5,
                       csBlockMatrix::Dynamic) {
  constexpr int B = csBlockMatrix::Variable;

  std::vector<int> blockSizes = {3, 2, 4, 4, 2};

  std::unique_ptr<csBlockMatrix::DimensionDescriptor<B, NB>> d;
  SECTION("Default ctor") {

    d.reset(new csBlockMatrix::DimensionDescriptor<B, NB>());
    REQUIRE(csBlockMatrix::DimensionDescriptor<B, NB>::Traits::numBlocksAtCompileTime == NB);

    d->resize(blockSizes, blockSizes.size());
    REQUIRE(d->numBlocks() == blockSizes.size());

    d->resize(blockSizes);
    REQUIRE(d->numBlocks() == blockSizes.size());
  }
  SECTION("Block-size ctor") {

    SECTION("vector only") {
      d.reset(new csBlockMatrix::DimensionDescriptor<B, NB>(blockSizes));
    }
    SECTION("vector + size only") {
      d.reset(new csBlockMatrix::DimensionDescriptor<B, NB>(blockSizes, int(blockSizes.size())));
    }

    REQUIRE(d->numBlocks() == 5);
  }

  int s = 0;
  for (int i = 0; i < d->numBlocks(); i++) {
    REQUIRE(d->blockSize(i) == blockSizes[i]);
    REQUIRE(d->blockStart(i) == s);
    s += d->blockSize(i);
  }

  REQUIRE(d->numElements() == s);

  if (csBlockMatrix::DimensionDescriptor<B, NB>::Traits::numBlocksAtCompileTime == csBlockMatrix::Dynamic) {
    int b = 8;
    blockSizes.push_back(b);
    d->addBlock(b);

    s = 0;
    for (int i = 0; i < d->numBlocks(); i++) {
      REQUIRE(d->blockSize(i) == blockSizes[i]);
      REQUIRE(d->blockStart(i) == s);
      s += d->blockSize(i);
    }

    REQUIRE(d->numElements() == s);
  }
}

TEST_CASE("DimensionDescriptorTraits", "[DimensionDescriptor]") {

  {
    using T = csBlockMatrix::DimensionDescriptorTraits<3, 5>;
    static_assert(std::is_same<T::BlockSizeType, int>::value, "");
    static_assert(T::blockSizeAtCompileTime == 3, "");
    static_assert(T::blockType == csBlockMatrix::Fixed, "");
    static_assert(T::numBlocksAtCompileTime == 5, "");
    static_assert(T::numElementsAtCompileTime == 15, "");
  }

  {
    using T = csBlockMatrix::DimensionDescriptorTraits<3, csBlockMatrix::Dynamic>;
    static_assert(std::is_same<T::BlockSizeType, int>::value, "");
    static_assert(T::blockSizeAtCompileTime == 3, "");
    static_assert(T::blockType == csBlockMatrix::Fixed, "");
    static_assert(T::numBlocksAtCompileTime == csBlockMatrix::Dynamic, "");
    static_assert(T::numElementsAtCompileTime == csBlockMatrix::Dynamic, "");
  }

  {
    using T = csBlockMatrix::DimensionDescriptorTraits<csBlockMatrix::Dynamic, 5>;
    static_assert(std::is_same<T::BlockSizeType, int>::value, "");
    static_assert(T::blockSizeAtCompileTime == csBlockMatrix::Dynamic, "");
    static_assert(T::blockType == csBlockMatrix::Dynamic, "");
    static_assert(T::numBlocksAtCompileTime == 5, "");
    static_assert(T::numElementsAtCompileTime == csBlockMatrix::Dynamic, "");
  }

  {
    using T = csBlockMatrix::DimensionDescriptorTraits<csBlockMatrix::Dynamic, csBlockMatrix::Dynamic>;
    static_assert(std::is_same<T::BlockSizeType, int>::value, "");
    static_assert(T::blockSizeAtCompileTime == csBlockMatrix::Dynamic, "");
    static_assert(T::blockType == csBlockMatrix::Dynamic, "");
    static_assert(T::numBlocksAtCompileTime == csBlockMatrix::Dynamic, "");
    static_assert(T::numElementsAtCompileTime == csBlockMatrix::Dynamic, "");
  }

  {
    using T = csBlockMatrix::DimensionDescriptorTraits<csBlockMatrix::Variable, 5>;
    static_assert(std::is_same<T::BlockSizeType, std::vector<int>>::value, "this should be std::vector<int>");
    static_assert(T::blockSizeAtCompileTime == csBlockMatrix::Dynamic, "");
    static_assert(T::blockType == csBlockMatrix::Variable, "");
    static_assert(T::numBlocksAtCompileTime == 5, "");
    static_assert(T::numElementsAtCompileTime == csBlockMatrix::Dynamic, "");
  }

  {
    using T = csBlockMatrix::DimensionDescriptorTraits<csBlockMatrix::Variable, csBlockMatrix::Dynamic>;
    static_assert(std::is_same<T::BlockSizeType, std::vector<int>>::value, "this should be std::vector<int>");
    static_assert(T::blockSizeAtCompileTime == csBlockMatrix::Dynamic, "");
    static_assert(T::blockType == csBlockMatrix::Variable, "");
    static_assert(T::numBlocksAtCompileTime == csBlockMatrix::Dynamic, "");
    static_assert(T::numElementsAtCompileTime == csBlockMatrix::Dynamic, "");
  }
}
