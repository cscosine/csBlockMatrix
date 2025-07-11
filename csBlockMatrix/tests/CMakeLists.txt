project(csBlockMatrixTest  LANGUAGES CXX)

cs_add_executable(csBlockMatrixTest)

cs_find_package(csBlockMatrixTest PUBLIC csCore REQUIRED CONFIG)
cs_find_package(csBlockMatrix PUBLIC csCore COMPONENTS testUtils math REQUIRED CONFIG)

include(CTest)
cs_find_package(csBlockMatrixTest PUBLIC Catch2 3 REQUIRED CONFIG)
include(Catch)

cs_target_sources(csBlockMatrixTest
	PRIVATE
  TestDenseMatrixBlock.cpp
  TestDenseMatrixBlockIterable.cpp
  TestDiagonalMatrixBlock.cpp
  TestDiagonalMatrixBlockIterable.cpp
  TestDimensionDescriptor.cpp
  TestMatrixBlockDescriptor.cpp
  TestSparseCoeffDiagonalMatrixBlock.cpp
  TestSparseCoeffDiagonalMatrixBlockIterable.cpp
  TestSparseCoeffMatrixBlock.cpp
  TestSparseMatrixBlock.cpp
  TestSparsityPatternBlockDescriptor.cpp
  TestSparsityPattern.cpp
  VectorBlockCatch.cpp
)

cs_target_link_libraries(csBlockMatrixTest
  PRIVATE
  csBlockMatrix::csBlockMatrix
  csCore::math
  csCore::testUtils
  csBlockMatrixLib::csBlockMatrixLib_csCMakeBaseInclude
  csBlockMatrixLib::csBlockMatrixLib_csCMakeCompileOptionsDefault
  Catch2::Catch2WithMain)

# add to tests
catch_discover_tests(csBlockMatrixTest DISCOVERY_MODE "PRE_TEST")

cs_install_target(csBlockMatrixTest csBlockMatrixTest)

cs_finalize_target(csBlockMatrixTest)
