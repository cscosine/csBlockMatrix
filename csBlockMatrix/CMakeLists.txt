project(csBlockMatrix LANGUAGES CXX)


# prepare custom project asserts, note: use standard find_package
find_package(csCore REQUIRED COMPONENTS assert CONFIG)

list(APPEND CMAKE_MODULE_PATH "${csCore_BASE_DIR}/cmake/csCore/assert/")
include(csAssert)

# set defaults for Paranoid additional configuration
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELEASE Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELWITHDEBINFO Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEBUG Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT PARANOID Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT SAFE_MATH Paranoid ON)

csAssertGenerateProject(csBlockMatrix csBlockMatrix) #generate target csBlockMatrix::csBlockMatrixAssert
list(POP_BACK CMAKE_MODULE_PATH)

# prepare standard library
cs_add_library(csBlockMatrix STATIC)
cs_add_library(csBlockMatrix::csBlockMatrix ALIAS csBlockMatrix)

cs_find_package(csBlockMatrix PUBLIC Eigen3 REQUIRED CONFIG)
cs_find_package(csBlockMatrix PUBLIC csCore COMPONENTS core eigenUtils REQUIRED CONFIG)
message(STATUS "Looking for OpenMP. If this fail, for clang try `sudo apt install libomp-dev`")
cs_find_package(csBlockMatrix PUBLIC OpenMP REQUIRED)

cs_target_sources(csBlockMatrix
	PRIVATE_INSTALL
    Global.h
		DimensionDescriptor.h
		DimensionDescriptor.hpp
    DimensionDescriptorTraits.h
    DimensionDescriptorTraits.hpp
    MatrixBlockDescriptor.h
    MatrixBlockDescriptor.hpp
    MatrixBlockTypeTraits.h
    MatrixTypeTraits.h
    DenseMatrixBlock.h
    DenseMatrixBlock.hpp
    DiagonalMatrixBlock.h
    DiagonalMatrixBlock.hpp
    MatrixBlockBase.h
    MatrixBlockBase.hpp
    SparsityPattern.h
    SparseMatrixBlock.h
    SparseMatrixBlock.hpp
    SparseCoeffMatrixBlock.h
    SparseCoeffMatrixBlock.hpp
    SparseCoeffDiagonalMatrixBlock.h
    SparseCoeffDiagonalMatrixBlock.hpp
    SparsityPatternBlockDescriptor.h
    SparsityPatternBlockDescriptor.hpp
    Utils.h
    Utils.hpp
    VectorBlock.h
    VectorBlock.hpp
		PRIVATE
    Global.cpp
		DimensionDescriptor.cpp
    DimensionDescriptorTraits.cpp
    MatrixBlockDescriptor.cpp
    DenseMatrixBlock.cpp
    DiagonalMatrixBlock.cpp
    MatrixBlockBase.cpp
    SparsityPattern.cpp
    SparseMatrixBlock.cpp
    SparsityPatternBlockDescriptor.cpp
    SparseCoeffMatrixBlock.cpp
    SparseCoeffDiagonalMatrixBlock.cpp
    Utils.cpp
    VectorBlock.cpp
)

cs_target_link_libraries(csBlockMatrix
	PUBLIC
		Eigen3::Eigen
    OpenMP::OpenMP_CXX
		csBlockMatrix::csBlockMatrixAssert
		csBlockMatrixLib::csBlockMatrixLib_csCMakeBaseInclude
)

add_subdirectory(tests)

cs_install_target(csBlockMatrix csBlockMatrix DEFAULT)

cs_finalize_target(csBlockMatrix)
