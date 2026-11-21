#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <csBlockMatrix/SparseCoeffDiagonalMatrixBlock_impl.hpp>

// let use W matrix in BA case
static constexpr int camSize = 6;
static constexpr int pointSize = 3;
static constexpr int numCams = 4;
static constexpr int numPoints = 10;

static const std::vector<int> camSizeVar = {5, 8, 6, 5};
static const std::vector<int> pointSizeVar = {2, 2, 3, 3, 4, 4, 5, 5, 1, 1};

TEMPLATE_TEST_CASE_SIG(
    "SparseCoeffDiagonalMatrixBlock", "[SparseCoeffDiagonalMatrixBlock]",
    ((int BR, int BC, int NBR, int NBC, int Ordering), BR, BC, NBR, NBC, Ordering),
    (camSize, pointSize, numCams, numPoints, csBlockMatrix::ColMajor),
    (camSize, pointSize, numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
    (camSize, pointSize, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
    (camSize, pointSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
    (camSize, csBlockMatrix::Dynamic, numCams, numPoints, csBlockMatrix::ColMajor),
    (camSize, csBlockMatrix::Dynamic, numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
    (camSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
    (camSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
    (csBlockMatrix::Dynamic, pointSize, numCams, numPoints, csBlockMatrix::ColMajor),
    (csBlockMatrix::Dynamic, pointSize, numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
    (csBlockMatrix::Dynamic, pointSize, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
    (csBlockMatrix::Dynamic, pointSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
    (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numCams, numPoints, csBlockMatrix::ColMajor),
    (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
    (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
    (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic,
     csBlockMatrix::ColMajor),
    //-----------------------------------------------------------
    (camSize, pointSize, numCams, numPoints, csBlockMatrix::RowMajor),
    (camSize, pointSize, numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
    (camSize, pointSize, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
    (camSize, pointSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
    (camSize, csBlockMatrix::Dynamic, numCams, numPoints, csBlockMatrix::RowMajor),
    (camSize, csBlockMatrix::Dynamic, numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
    (camSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
    (camSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
    (csBlockMatrix::Dynamic, pointSize, numCams, numPoints, csBlockMatrix::RowMajor),
    (csBlockMatrix::Dynamic, pointSize, numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
    (csBlockMatrix::Dynamic, pointSize, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
    (csBlockMatrix::Dynamic, pointSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
    (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numCams, numPoints, csBlockMatrix::RowMajor),
    (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
    (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
    (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic,
     csBlockMatrix::RowMajor)) {

  using MatT = csBlockMatrix::SparseCoeffDiagonalMatrixBlock<double, Ordering, BR, BC, NBR, NBC>;

  auto sp = typename csBlockMatrix::SparsityPattern<Ordering>::SPtr(
      new csBlockMatrix::SparsityPattern<Ordering>(numCams, numCams));
  sp->setDiagonal();

  std::unique_ptr<MatT> mat;
  SECTION("default ctor") {
    mat.reset(new MatT());
    REQUIRE(mat->numBlocksRow() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));
    REQUIRE(mat->numBlocksCol() == (NBC == csBlockMatrix::Dynamic ? 0 : NBC));

    mat->resize(typename MatT::BlockDescriptor(camSize, numCams, pointSize, numPoints), sp);
  }
  SECTION("sized ctor") {
    mat.reset(new MatT(typename MatT::BlockDescriptor(camSize, pointSize), sp));
    mat->resize(typename MatT::BlockDescriptor(camSize, numCams, pointSize, numPoints), sp);
  }
  SECTION("sized ctor") {
    mat.reset(new MatT(typename MatT::BlockDescriptor(camSize, numCams, pointSize, numPoints), sp));
    mat->resize(typename MatT::BlockDescriptor(camSize, numCams, pointSize, numPoints), sp);
  }

  REQUIRE(mat->numElementRows() == camSize * numCams);
  REQUIRE(mat->numElementCols() == pointSize * numPoints);

  MatT mat2(mat->blockDescriptor(), sp);
  REQUIRE(mat2.numElementRows() == camSize * numCams);
  REQUIRE(mat2.numElementCols() == pointSize * numPoints);

  MatT mat3(typename MatT::BlockDescriptor(mat->blockDescriptor().rowDescriptionCSPtr(),
                                           mat->blockDescriptor().colDescriptionCSPtr()),
            sp);
  REQUIRE(mat3.numElementRows() == camSize * numCams);
  REQUIRE(mat3.numElementCols() == pointSize * numPoints);

  MatT mat4;
  mat4.resize(typename MatT::BlockDescriptor(mat->blockDescriptor().rowDescriptionCSPtr(),
                                             mat->blockDescriptor().colDescriptionCSPtr()),
              sp);
  REQUIRE(mat4.numElementRows() == camSize * numCams);
  REQUIRE(mat4.numElementCols() == pointSize * numPoints);

  MatT mat5;
  mat5.resize(mat->blockDescriptor(), sp);
  REQUIRE(mat5.numElementRows() == camSize * numCams);
  REQUIRE(mat5.numElementCols() == pointSize * numPoints);

  for (int i = 0; i < mat->numBlocksRow(); i++) {
    REQUIRE(mat->rowBlockSize(i) == camSize);
    REQUIRE(mat2.rowBlockSize(i) == camSize);
    REQUIRE(mat3.rowBlockSize(i) == camSize);
    REQUIRE(mat4.rowBlockSize(i) == camSize);
    REQUIRE(mat5.rowBlockSize(i) == camSize);
  }
  for (int i = 0; i < mat->numBlocksCol(); i++) {
    REQUIRE(mat->colBlockSize(i) == pointSize);
    REQUIRE(mat2.colBlockSize(i) == pointSize);
    REQUIRE(mat3.colBlockSize(i) == pointSize);
    REQUIRE(mat4.colBlockSize(i) == pointSize);
    REQUIRE(mat5.colBlockSize(i) == pointSize);
  }
}

TEMPLATE_TEST_CASE_SIG("SparseCoeffDiagonalMatrixBlock-Square", "[SparseCoeffDiagonalMatrixBlock]",
                       ((int BR, int NBR, int Ordering), BR, NBR, Ordering),
                       (camSize, numCams, csBlockMatrix::ColMajor),
                       (camSize, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, numCams, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       //---------------------------------------------
                       (camSize, numCams, csBlockMatrix::RowMajor),
                       (camSize, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, numCams, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor)) {

  using MatT = csBlockMatrix::SparseCoeffDiagonalMatrixBlock<double, Ordering, BR, BR, NBR, NBR>;

  auto sp = typename csBlockMatrix::SparsityPattern<Ordering>::SPtr(
      new csBlockMatrix::SparsityPattern<Ordering>(numCams, numCams));
  sp->setDiagonal();

  std::unique_ptr<MatT> mat;
  SECTION("default ctor") {
    mat.reset(new MatT());
    REQUIRE(mat->numBlocksRow() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));
    REQUIRE(mat->numBlocksCol() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));

    mat->resize(MatT::BlockDescriptor::squareMatrix(camSize, numCams), sp);
  }
  SECTION("sized ctor") {
    mat.reset(new MatT(MatT::BlockDescriptor::squareMatrix(camSize), sp));
    mat->resize(MatT::BlockDescriptor::squareMatrix(camSize, numCams), sp);
  }
  SECTION("sized ctor") {
    mat.reset(new MatT(MatT::BlockDescriptor::squareMatrix(camSize, numCams), sp));
    mat->resize(MatT::BlockDescriptor::squareMatrix(camSize, numCams), sp);
  }

  REQUIRE(mat->numElementRows() == camSize * numCams);
  REQUIRE(mat->numElementCols() == camSize * numCams);

  MatT mat2(MatT::BlockDescriptor::squareMatrix(mat->blockDescriptor().rowDescriptionCSPtr()), sp);
  REQUIRE(mat2.numElementRows() == camSize * numCams);
  REQUIRE(mat2.numElementCols() == camSize * numCams);

  MatT mat3(typename MatT::BlockDescriptor(mat->blockDescriptor().rowDescriptionCSPtr(),
                                           mat->blockDescriptor().colDescriptionCSPtr()),
            sp);
  REQUIRE(mat3.numElementRows() == camSize * numCams);
  REQUIRE(mat3.numElementCols() == camSize * numCams);

  MatT mat4;
  mat4.resize(typename MatT::BlockDescriptor(mat->blockDescriptor().rowDescriptionCSPtr(),
                                             mat->blockDescriptor().colDescriptionCSPtr()),
              sp);
  REQUIRE(mat4.numElementRows() == camSize * numCams);
  REQUIRE(mat4.numElementCols() == camSize * numCams);

  MatT mat5;
  mat5.resize(mat->blockDescriptor(), sp);
  REQUIRE(mat5.numElementRows() == camSize * numCams);
  REQUIRE(mat5.numElementCols() == camSize * numCams);

  for (int i = 0; i < mat->numBlocksRow(); i++) {
    REQUIRE(mat->rowBlockSize(i) == camSize);
    REQUIRE(mat2.rowBlockSize(i) == camSize);
    REQUIRE(mat3.rowBlockSize(i) == camSize);
    REQUIRE(mat4.rowBlockSize(i) == camSize);
    REQUIRE(mat5.rowBlockSize(i) == camSize);
  }
  for (int i = 0; i < mat->numBlocksCol(); i++) {
    REQUIRE(mat->colBlockSize(i) == camSize);
    REQUIRE(mat2.colBlockSize(i) == camSize);
    REQUIRE(mat3.colBlockSize(i) == camSize);
    REQUIRE(mat4.colBlockSize(i) == camSize);
    REQUIRE(mat5.colBlockSize(i) == camSize);
  }
}

TEMPLATE_TEST_CASE_SIG("SparseCoeffDiagonalMatrixBlockVar", "[SparseCoeffDiagonalMatrixBlock]",
                       ((int NBR, int NBC, int Ordering), NBR, NBC, Ordering),
                       (numCams, numPoints, csBlockMatrix::ColMajor),
                       (numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       //----------------------------
                       (numCams, numPoints, csBlockMatrix::RowMajor),
                       (numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor)) {
  using MatT = csBlockMatrix::SparseCoeffDiagonalMatrixBlock<double, Ordering, csBlockMatrix::Variable,
                                                             csBlockMatrix::Variable, NBR, NBC>;
  std::unique_ptr<MatT> mat;
  auto sp = typename csBlockMatrix::SparsityPattern<Ordering>::SPtr(
      new csBlockMatrix::SparsityPattern<Ordering>(numCams, numCams));
  sp->setDiagonal();

  SECTION("default ctor") {
    mat.reset(new MatT());
    REQUIRE(mat->numBlocksRow() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));
    REQUIRE(mat->numBlocksCol() == (NBC == csBlockMatrix::Dynamic ? 0 : NBC));

    mat->resize(typename MatT::BlockDescriptor(camSizeVar, numCams, pointSizeVar, numPoints), sp);
  }

  SECTION("sized ctor") {
    mat.reset(new MatT(typename MatT::BlockDescriptor(camSizeVar, pointSizeVar), sp));
    mat->resize(typename MatT::BlockDescriptor(camSizeVar, numCams, pointSizeVar, numPoints), sp);
  }
  SECTION("sized ctor") {
    mat.reset(new MatT(typename MatT::BlockDescriptor(camSizeVar, numCams, pointSizeVar, numPoints), sp));
    mat->resize(typename MatT::BlockDescriptor(camSizeVar, numCams, pointSizeVar, numPoints), sp);
  }

  REQUIRE(mat->numElementRows() == camSize * numCams);
  REQUIRE(mat->numElementCols() == pointSize * numPoints);

  MatT mat2(typename MatT::BlockDescriptor(mat->blockDescriptor()), sp);
  REQUIRE(mat2.numElementRows() == camSize * numCams);
  REQUIRE(mat2.numElementCols() == pointSize * numPoints);

  MatT mat3(typename MatT::BlockDescriptor(mat->blockDescriptor().rowDescriptionCSPtr(),
                                           mat->blockDescriptor().colDescriptionCSPtr()),
            sp);
  REQUIRE(mat3.numElementRows() == camSize * numCams);
  REQUIRE(mat3.numElementCols() == pointSize * numPoints);

  MatT mat4;
  mat4.resize(typename MatT::BlockDescriptor(mat->blockDescriptor().rowDescriptionCSPtr(),
                                             mat->blockDescriptor().colDescriptionCSPtr()),
              sp);
  REQUIRE(mat4.numElementRows() == camSize * numCams);
  REQUIRE(mat4.numElementCols() == pointSize * numPoints);

  MatT mat5;
  mat5.resize(typename MatT::BlockDescriptor(mat->blockDescriptor()), sp);
  REQUIRE(mat5.numElementRows() == camSize * numCams);
  REQUIRE(mat5.numElementCols() == pointSize * numPoints);

  for (int i = 0; i < mat->numBlocksRow(); i++) {
    REQUIRE(mat->rowBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat2.rowBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat3.rowBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat4.rowBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat5.rowBlockSize(i) == camSizeVar[i]);
  }
  for (int i = 0; i < mat->numBlocksCol(); i++) {
    REQUIRE(mat->colBlockSize(i) == pointSizeVar[i]);
    REQUIRE(mat2.colBlockSize(i) == pointSizeVar[i]);
    REQUIRE(mat3.colBlockSize(i) == pointSizeVar[i]);
    REQUIRE(mat4.colBlockSize(i) == pointSizeVar[i]);
    REQUIRE(mat5.colBlockSize(i) == pointSizeVar[i]);
  }
}

TEMPLATE_TEST_CASE_SIG("SparseCoeffDiagonalMatrixBlockVar-Square", "[SparseCoeffDiagonalMatrixBlock]",
                       ((int NBR, int Ordering), NBR, Ordering), (numCams, csBlockMatrix::ColMajor),
                       (numCams, csBlockMatrix::RowMajor), (csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::RowMajor)) {
  using MatT = csBlockMatrix::SparseCoeffDiagonalMatrixBlock<double, Ordering, csBlockMatrix::Variable,
                                                             csBlockMatrix::Variable, NBR, NBR>;
  std::unique_ptr<MatT> mat;

  auto sp = typename csBlockMatrix::SparsityPattern<Ordering>::SPtr(
      new csBlockMatrix::SparsityPattern<Ordering>(numCams, numCams));
  sp->setDiagonal();

  SECTION("default ctor") {
    mat.reset(new MatT());
    REQUIRE(mat->numBlocksRow() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));
    REQUIRE(mat->numBlocksCol() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));

    REQUIRE(mat->nonZeroBlocks() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));

    auto block = typename MatT::BlockDescriptor(camSizeVar, numCams, camSizeVar, numCams);
    mat->resize(block, sp);
  }

  SECTION("sized ctor") {
    auto block = MatT::BlockDescriptor::squareMatrix(camSizeVar);
    mat.reset(new MatT(block, sp));
    block.resizeSquare(camSizeVar, numCams);
    mat->resize(block, sp);
  }
  SECTION("sized ctor") {
    auto block = MatT::BlockDescriptor::squareMatrix(camSizeVar);
    mat.reset(new MatT(block, sp));
    block.resizeSquare(camSizeVar, numCams);
    mat->resize(block, sp);
  }

  REQUIRE(mat->numElementRows() == camSize * numCams);
  REQUIRE(mat->numElementCols() == camSize * numCams);

  REQUIRE(mat->mat().rows() == camSize * numCams);
  REQUIRE(mat->mat().cols() == camSize * numCams);

  MatT mat2(mat->blockDescriptor(), sp);
  REQUIRE(std::min(mat2.numBlocksRow(), mat2.numBlocksCol()) == mat->nonZeroBlocks());

  MatT mat3(typename MatT::BlockDescriptor(mat->blockDescriptor().rowDescriptionCSPtr(),
                                           mat->blockDescriptor().colDescriptionCSPtr()),
            sp);
  REQUIRE(std::min(mat3.numBlocksRow(), mat3.numBlocksCol()) == mat->nonZeroBlocks());

  MatT mat4;
  mat4.resize(typename MatT::BlockDescriptor(mat->blockDescriptor().rowDescriptionCSPtr(),
                                             mat->blockDescriptor().colDescriptionCSPtr()),
              sp);
  REQUIRE(std::min(mat4.numBlocksRow(), mat4.numBlocksCol()) == mat->nonZeroBlocks());

  MatT mat5;
  mat5.resize(typename MatT::BlockDescriptor(mat->blockDescriptor()), sp);
  REQUIRE(std::min(mat5.numBlocksRow(), mat5.numBlocksCol()) == mat->nonZeroBlocks());

  for (int i = 0; i < std::min(mat->numBlocksRow(), mat->numBlocksCol()); i++) {
    REQUIRE(mat->rowBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat2.rowBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat3.rowBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat4.rowBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat5.rowBlockSize(i) == camSizeVar[i]);

    REQUIRE(mat->colBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat2.colBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat3.colBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat4.colBlockSize(i) == camSizeVar[i]);
    REQUIRE(mat5.colBlockSize(i) == camSizeVar[i]);
  }
}
