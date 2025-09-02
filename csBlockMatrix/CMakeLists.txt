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

message(STATUS "Looking for OpenMP. If this fail, for clang on linux try `sudo apt install libomp-dev`")
# TODO(sc) still not working on win MSVC+LLVM
# seems related https://gitlab.kitware.com/cmake/cmake/-/issues/20999
cs_find_package(csBlockMatrix PUBLIC OpenMP REQUIRED)

cs_target_sources(csBlockMatrix
	PRIVATE_INSTALL
    Global.hpp
		DimensionDescriptor.hpp
		DimensionDescriptor_impl.hpp
    DimensionDescriptorTraits.hpp
    DimensionDescriptorTraits_impl.hpp
    MatrixBlockDescriptor.hpp
    MatrixBlockDescriptor_impl.hpp
    MatrixBlockTypeTraits.hpp
    MatrixTypeTraits.hpp
    DenseMatrixBlock.hpp
    DenseMatrixBlock_impl.hpp
    DiagonalMatrixBlock.hpp
    DiagonalMatrixBlock_impl.hpp
    MatrixBlockBase.hpp
    MatrixBlockBase_impl.hpp
    SparsityPattern.hpp
    SparseMatrixBlock.hpp
    SparseMatrixBlock_impl.hpp
    SparseCoeffMatrixBlock.hpp
    SparseCoeffMatrixBlock_impl.hpp
    SparseCoeffDiagonalMatrixBlock.hpp
    SparseCoeffDiagonalMatrixBlock_impl.hpp
    SparsityPatternBlockDescriptor.hpp
    SparsityPatternBlockDescriptor_impl.hpp
    Utils.hpp
    Utils_impl.hpp
    VectorBlock.hpp
    VectorBlock_impl.hpp
		PRIVATE
		DimensionDescriptor.cpp
    DimensionDescriptorTraits.cpp
    SparsityPattern.cpp
)

cs_target_link_libraries(csBlockMatrix
	PUBLIC
		Eigen3::Eigen
    OpenMP::OpenMP_CXX
		csBlockMatrix::csBlockMatrixAssert
		csBlockMatrixSol::csBlockMatrixSol_csCMakeBaseInclude
    csBlockMatrixSol::csBlockMatrixSol_csCMakeCompileOptionsDefault
)

add_subdirectory(tests)

cs_install_target(csBlockMatrix csBlockMatrix DEFAULT)

cs_finalize_target(csBlockMatrix)
