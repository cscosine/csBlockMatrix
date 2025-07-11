#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "csBlockMatrix/SparsityPatternBlockDescriptor.hpp"

// let use W matrix in BA case
static constexpr int camSize = 6;
static constexpr int pointSize = 3;
static constexpr int numCams = 3;
static constexpr int numPoints = 5;

static const std::vector<int> camSizeVar = {5, 7, 6};
static const std::vector<int> pointSizeVar = {2, 3, 4, 5, 1};

TEMPLATE_TEST_CASE_SIG("SparsityPatternBlockDescriptor", "[SparsityPatternBlockDescriptor]",
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

  using MatDescT = csBlockMatrix::MatrixBlockDescriptor<BR, BC, NBR, NBC>;

  std::unique_ptr<MatDescT> mat;
  SECTION("default ctor") {
    mat.reset(new MatDescT());
    REQUIRE(mat->numBlocksRow() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));
    REQUIRE(mat->numBlocksCol() == (NBC == csBlockMatrix::Dynamic ? 0 : NBC));
    mat->resize(camSize, numCams, pointSize, numPoints);
  }
  SECTION("sized ctor") {
    mat.reset(new MatDescT(camSize, pointSize));
    mat->resize(camSize, numCams, pointSize, numPoints);
  }
  SECTION("sized ctor") {
    mat.reset(new MatDescT(camSize, numCams, pointSize, numPoints));
    mat->resize(camSize, numCams, pointSize, numPoints);
  }

  REQUIRE(mat->numElementRows() == camSize * numCams);
  REQUIRE(mat->numElementCols() == pointSize * numPoints);

  for (int i = 0; i < mat->numBlocksRow(); i++) {
    REQUIRE(mat->rowBlockSize(i) == camSize);
  }
  for (int i = 0; i < mat->numBlocksCol(); i++) {
    REQUIRE(mat->colBlockSize(i) == pointSize);
  }

  csBlockMatrix::SparsityPattern<Ordering> sp(numCams, numPoints);
  /*
  +-------------------+
  | X |   | X | X |   |
  +-------------------+
  |   | X |   |   | X |
  +-------------------+
  | X | X |   |   | X |
  */
  sp.add(0, 0);
  sp.add(0, 2);
  sp.add(0, 3);
  sp.add(1, 1);
  sp.add(1, 4);
  sp.add(2, 0);
  sp.add(2, 1);
  sp.add(2, 4);

  csBlockMatrix::SparsityPatternBlockDescriptor<Ordering> spbd(sp, *mat);

  if (Ordering == csBlockMatrix::ColMajor) {
    REQUIRE(sp.outerSize() == numPoints);
    REQUIRE(spbd.outerSize() == numPoints);

    REQUIRE(spbd.stride(0) == mat->rowBlockSize(0) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(0, 0) == 0);
    REQUIRE(spbd.offset(0, 1) == mat->rowBlockSize(0));

    int numEl = mat->colBlockSize(0) * (mat->rowBlockSize(0) + mat->rowBlockSize(2));

    REQUIRE(spbd.stride(1) == mat->rowBlockSize(1) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(1, 0) == numEl);
    REQUIRE(spbd.offset(1, 1) == numEl + mat->rowBlockSize(1));
    numEl += mat->colBlockSize(1) * (mat->rowBlockSize(1) + mat->rowBlockSize(2));

    REQUIRE(spbd.stride(2) == mat->rowBlockSize(0));
    REQUIRE(spbd.offset(2, 0) == numEl);
    numEl += mat->colBlockSize(2) * mat->rowBlockSize(0);

    REQUIRE(spbd.stride(3) == mat->rowBlockSize(0));
    REQUIRE(spbd.offset(3, 0) == numEl);
    numEl += mat->colBlockSize(3) * mat->rowBlockSize(0);

    REQUIRE(spbd.stride(4) == mat->rowBlockSize(1) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(4, 0) == numEl);
    REQUIRE(spbd.offset(4, 1) == numEl + mat->rowBlockSize(1));
    numEl += mat->colBlockSize(4) * (mat->rowBlockSize(1) + mat->rowBlockSize(2));

    REQUIRE(numEl == spbd.nonZeroCoeffs());

  } else if (Ordering == csBlockMatrix::RowMajor) {
    REQUIRE(sp.outerSize() == numCams);
    REQUIRE(spbd.outerSize() == numCams);

    REQUIRE(spbd.stride(0) == mat->colBlockSize(0) + mat->colBlockSize(2) + mat->colBlockSize(3));

    REQUIRE(spbd.offset(0, 0) == 0);
    REQUIRE(spbd.offset(0, 1) == mat->colBlockSize(0));
    REQUIRE(spbd.offset(0, 2) == mat->colBlockSize(0) + mat->colBlockSize(2));

    int numEl = mat->rowBlockSize(0) * (mat->colBlockSize(0) + mat->colBlockSize(2) + mat->colBlockSize(3));

    REQUIRE(spbd.stride(1) == mat->colBlockSize(1) + mat->colBlockSize(4));
    REQUIRE(spbd.offset(1, 0) == numEl);
    REQUIRE(spbd.offset(1, 1) == numEl + mat->colBlockSize(1));
    numEl += mat->rowBlockSize(1) * (mat->colBlockSize(1) + mat->colBlockSize(4));

    REQUIRE(spbd.stride(2) == mat->colBlockSize(0) + mat->colBlockSize(1) + mat->colBlockSize(4));
    REQUIRE(spbd.offset(2, 0) == numEl);
    REQUIRE(spbd.offset(2, 1) == numEl + mat->colBlockSize(0));
    REQUIRE(spbd.offset(2, 2) == numEl + mat->colBlockSize(0) + mat->colBlockSize(1));
    numEl += mat->rowBlockSize(2) * (mat->colBlockSize(0) + mat->colBlockSize(1) + mat->colBlockSize(4));

    REQUIRE(numEl == spbd.nonZeroCoeffs());
  }
}

TEMPLATE_TEST_CASE_SIG("SparsityPatternBlockDescriptor-Var", "[MatrixBlockDescriptor]",
                       ((int NBR, int NBC, int Ordering), NBR, NBC, Ordering), (numCams, numPoints, csBlockMatrix::ColMajor),
                       (numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (numCams, numPoints, csBlockMatrix::RowMajor), (numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor)) {
  using MatDescT = csBlockMatrix::MatrixBlockDescriptor<csBlockMatrix::Variable, csBlockMatrix::Variable, NBR, NBC>;
  std::unique_ptr<MatDescT> mat;
  SECTION("default ctor") {
    mat.reset(new MatDescT());
    REQUIRE(mat->numBlocksRow() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));
    REQUIRE(mat->numBlocksCol() == (NBC == csBlockMatrix::Dynamic ? 0 : NBC));
    mat->resize(camSizeVar, numCams, pointSizeVar, numPoints);
  }
  SECTION("sized ctor") {
    mat.reset(new MatDescT(camSizeVar, pointSizeVar));
    mat->resize(camSizeVar, numCams, pointSizeVar, numPoints);
  }
  SECTION("sized ctor") {
    mat.reset(new MatDescT(camSizeVar, numCams, pointSizeVar, numPoints));
    mat->resize(camSizeVar, numCams, pointSizeVar, numPoints);
  }

  REQUIRE(mat->numElementRows() == camSize * numCams); // vector are same size on purpose!
  REQUIRE(mat->numElementCols() == pointSize * numPoints);

  for (int i = 0; i < mat->numBlocksRow(); i++) {
    REQUIRE(mat->rowBlockSize(i) == camSizeVar[i]);
  }
  for (int i = 0; i < mat->numBlocksCol(); i++) {
    REQUIRE(mat->colBlockSize(i) == pointSizeVar[i]);
  }

  csBlockMatrix::SparsityPattern<Ordering> sp(numCams, numPoints);
  /*
  +-------------------+
  | X |   | X | X |   |
  +-------------------+
  |   | X |   |   | X |
  +-------------------+
  | X | X |   |   | X |
  */
  sp.add(0, 0);
  sp.add(0, 2);
  sp.add(0, 3);
  sp.add(1, 1);
  sp.add(1, 4);
  sp.add(2, 0);
  sp.add(2, 1);
  sp.add(2, 4);

  csBlockMatrix::SparsityPatternBlockDescriptor<Ordering> spbd(sp, *mat);

  if (Ordering == csBlockMatrix::ColMajor) {
    REQUIRE(sp.outerSize() == numPoints);
    REQUIRE(spbd.outerSize() == numPoints);

    REQUIRE(spbd.stride(0) == mat->rowBlockSize(0) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(0, 0) == 0);
    REQUIRE(spbd.offset(0, 1) == mat->rowBlockSize(0));

    int numEl = mat->colBlockSize(0) * (mat->rowBlockSize(0) + mat->rowBlockSize(2));

    REQUIRE(spbd.stride(1) == mat->rowBlockSize(1) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(1, 0) == numEl);
    REQUIRE(spbd.offset(1, 1) == numEl + mat->rowBlockSize(1));
    numEl += mat->colBlockSize(1) * (mat->rowBlockSize(1) + mat->rowBlockSize(2));

    REQUIRE(spbd.stride(2) == mat->rowBlockSize(0));
    REQUIRE(spbd.offset(2, 0) == numEl);
    numEl += mat->colBlockSize(2) * mat->rowBlockSize(0);

    REQUIRE(spbd.stride(3) == mat->rowBlockSize(0));
    REQUIRE(spbd.offset(3, 0) == numEl);
    numEl += mat->colBlockSize(3) * mat->rowBlockSize(0);

    REQUIRE(spbd.stride(4) == mat->rowBlockSize(1) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(4, 0) == numEl);
    REQUIRE(spbd.offset(4, 1) == numEl + mat->rowBlockSize(1));
    numEl += mat->colBlockSize(4) * (mat->rowBlockSize(1) + mat->rowBlockSize(2));

    REQUIRE(numEl == spbd.nonZeroCoeffs());
  } else if (Ordering == csBlockMatrix::RowMajor) {
    REQUIRE(sp.outerSize() == numCams);
    REQUIRE(spbd.outerSize() == numCams);

    REQUIRE(spbd.stride(0) == mat->colBlockSize(0) + mat->colBlockSize(2) + mat->colBlockSize(3));

    REQUIRE(spbd.offset(0, 0) == 0);
    REQUIRE(spbd.offset(0, 1) == mat->colBlockSize(0));
    REQUIRE(spbd.offset(0, 2) == mat->colBlockSize(0) + mat->colBlockSize(2));

    int numEl = mat->rowBlockSize(0) * (mat->colBlockSize(0) + mat->colBlockSize(2) + mat->colBlockSize(3));

    REQUIRE(spbd.stride(1) == mat->colBlockSize(1) + mat->colBlockSize(4));
    REQUIRE(spbd.offset(1, 0) == numEl);
    REQUIRE(spbd.offset(1, 1) == numEl + mat->colBlockSize(1));
    numEl += mat->rowBlockSize(1) * (mat->colBlockSize(1) + mat->colBlockSize(4));

    REQUIRE(spbd.stride(2) == mat->colBlockSize(0) + mat->colBlockSize(1) + mat->colBlockSize(4));
    REQUIRE(spbd.offset(2, 0) == numEl);
    REQUIRE(spbd.offset(2, 1) == numEl + mat->colBlockSize(0));
    REQUIRE(spbd.offset(2, 2) == numEl + mat->colBlockSize(0) + mat->colBlockSize(1));
    numEl += mat->rowBlockSize(2) * (mat->colBlockSize(0) + mat->colBlockSize(1) + mat->colBlockSize(4));

    REQUIRE(numEl == spbd.nonZeroCoeffs());
  }
}

TEMPLATE_TEST_CASE_SIG("SparsityPatternBlockDescriptor-VarFix", "[MatrixBlockDescriptor]",
                       ((int BC, int NBR, int NBC, int Ordering), BC, NBR, NBC, Ordering),
                       (pointSize, numCams, numPoints, csBlockMatrix::ColMajor),
                       (pointSize, numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (pointSize, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
                       (pointSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (pointSize, numCams, numPoints, csBlockMatrix::RowMajor),
                       (pointSize, numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (pointSize, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
                       (pointSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, numCams, numPoints, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, numCams, numPoints, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor)) {
  using MatDescT = csBlockMatrix::MatrixBlockDescriptor<csBlockMatrix::Variable, BC, NBR, NBC>;
  std::unique_ptr<MatDescT> mat;
  SECTION("default ctor") {
    mat.reset(new MatDescT());
    REQUIRE(mat->numBlocksRow() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));
    REQUIRE(mat->numBlocksCol() == (NBC == csBlockMatrix::Dynamic ? 0 : NBC));
    mat->resize(camSizeVar, numCams, pointSize, numPoints);
  }
  SECTION("sized ctor") {
    mat.reset(new MatDescT(camSizeVar, pointSize));
    mat->resize(camSizeVar, numCams, pointSize, numPoints);
  }
  SECTION("sized ctor") {
    mat.reset(new MatDescT(camSizeVar, numCams, pointSize, numPoints));
    mat->resize(camSizeVar, numCams, pointSize, numPoints);
  }

  REQUIRE(mat->numElementRows() == camSize * numCams); // vector are same size on purpose!
  REQUIRE(mat->numElementCols() == pointSize * numPoints);

  for (int i = 0; i < mat->numBlocksRow(); i++) {
    REQUIRE(mat->rowBlockSize(i) == camSizeVar[i]);
  }
  for (int i = 0; i < mat->numBlocksCol(); i++) {
    REQUIRE(mat->colBlockSize(i) == pointSize);
  }

  csBlockMatrix::SparsityPattern<Ordering> sp(numCams, numPoints);
  /*
  +-------------------+
  | X |   | X | X |   |
  +-------------------+
  |   | X |   |   | X |
  +-------------------+
  | X | X |   |   | X |
  */
  sp.add(0, 0);
  sp.add(0, 2);
  sp.add(0, 3);
  sp.add(1, 1);
  sp.add(1, 4);
  sp.add(2, 0);
  sp.add(2, 1);
  sp.add(2, 4);

  csBlockMatrix::SparsityPatternBlockDescriptor<Ordering> spbd(sp, *mat);

  if (Ordering == csBlockMatrix::ColMajor) {
    REQUIRE(sp.outerSize() == numPoints);
    REQUIRE(spbd.outerSize() == numPoints);

    REQUIRE(spbd.stride(0) == mat->rowBlockSize(0) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(0, 0) == 0);
    REQUIRE(spbd.offset(0, 1) == mat->rowBlockSize(0));

    int numEl = mat->colBlockSize(0) * (mat->rowBlockSize(0) + mat->rowBlockSize(2));

    REQUIRE(spbd.stride(1) == mat->rowBlockSize(1) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(1, 0) == numEl);
    REQUIRE(spbd.offset(1, 1) == numEl + mat->rowBlockSize(1));
    numEl += mat->colBlockSize(1) * (mat->rowBlockSize(1) + mat->rowBlockSize(2));

    REQUIRE(spbd.stride(2) == mat->rowBlockSize(0));
    REQUIRE(spbd.offset(2, 0) == numEl);
    numEl += mat->colBlockSize(2) * mat->rowBlockSize(0);

    REQUIRE(spbd.stride(3) == mat->rowBlockSize(0));
    REQUIRE(spbd.offset(3, 0) == numEl);
    numEl += mat->colBlockSize(3) * mat->rowBlockSize(0);

    REQUIRE(spbd.stride(4) == mat->rowBlockSize(1) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(4, 0) == numEl);
    REQUIRE(spbd.offset(4, 1) == numEl + mat->rowBlockSize(1));
    numEl += mat->colBlockSize(4) * (mat->rowBlockSize(1) + mat->rowBlockSize(2));

    REQUIRE(numEl == spbd.nonZeroCoeffs());
  } else if (Ordering == csBlockMatrix::RowMajor) {
    REQUIRE(sp.outerSize() == numCams);
    REQUIRE(spbd.outerSize() == numCams);

    REQUIRE(spbd.stride(0) == mat->colBlockSize(0) + mat->colBlockSize(2) + mat->colBlockSize(3));

    REQUIRE(spbd.offset(0, 0) == 0);
    REQUIRE(spbd.offset(0, 1) == mat->colBlockSize(0));
    REQUIRE(spbd.offset(0, 2) == mat->colBlockSize(0) + mat->colBlockSize(2));

    int numEl = mat->rowBlockSize(0) * (mat->colBlockSize(0) + mat->colBlockSize(2) + mat->colBlockSize(3));

    REQUIRE(spbd.stride(1) == mat->colBlockSize(1) + mat->colBlockSize(4));
    REQUIRE(spbd.offset(1, 0) == numEl);
    REQUIRE(spbd.offset(1, 1) == numEl + mat->colBlockSize(1));
    numEl += mat->rowBlockSize(1) * (mat->colBlockSize(1) + mat->colBlockSize(4));

    REQUIRE(spbd.stride(2) == mat->colBlockSize(0) + mat->colBlockSize(1) + mat->colBlockSize(4));
    REQUIRE(spbd.offset(2, 0) == numEl);
    REQUIRE(spbd.offset(2, 1) == numEl + mat->colBlockSize(0));
    REQUIRE(spbd.offset(2, 2) == numEl + mat->colBlockSize(0) + mat->colBlockSize(1));
    numEl += mat->rowBlockSize(2) * (mat->colBlockSize(0) + mat->colBlockSize(1) + mat->colBlockSize(4));

    REQUIRE(numEl == spbd.nonZeroCoeffs());
  }
}

TEMPLATE_TEST_CASE_SIG("SparsityPatternBlockDescriptor-FixVar", "[MatrixBlockDescriptor]",
                       ((int BR, int NBR, int NBC, int Ordering), BR, NBR, NBC, Ordering),
                       (camSize, numCams, numPoints, csBlockMatrix::ColMajor),
                       (camSize, numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (camSize, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
                       (camSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (camSize, numCams, numPoints, csBlockMatrix::RowMajor),
                       (camSize, numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (camSize, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
                       (camSize, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, numCams, numPoints, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, numCams, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::ColMajor),
                       (csBlockMatrix::Dynamic, numCams, numPoints, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, numCams, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, numPoints, csBlockMatrix::RowMajor),
                       (csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::Dynamic, csBlockMatrix::RowMajor)) {
  using MatDescT = csBlockMatrix::MatrixBlockDescriptor<BR, csBlockMatrix::Variable, NBR, NBC>;
  std::unique_ptr<MatDescT> mat;
  SECTION("default ctor") {
    mat.reset(new MatDescT());
    REQUIRE(mat->numBlocksRow() == (NBR == csBlockMatrix::Dynamic ? 0 : NBR));
    REQUIRE(mat->numBlocksCol() == (NBC == csBlockMatrix::Dynamic ? 0 : NBC));
    mat->resize(camSize, numCams, pointSizeVar, numPoints);
  }
  SECTION("sized ctor") {
    mat.reset(new MatDescT(camSize, pointSizeVar));
    mat->resize(camSize, numCams, pointSizeVar, numPoints);
  }
  SECTION("sized ctor") {
    mat.reset(new MatDescT(camSize, numCams, pointSizeVar, numPoints));
    mat->resize(camSize, numCams, pointSizeVar, numPoints);
  }

  REQUIRE(mat->numElementRows() == camSize * numCams); // vector are same size on purpose!
  REQUIRE(mat->numElementCols() == pointSize * numPoints);

  for (int i = 0; i < mat->numBlocksRow(); i++) {
    REQUIRE(mat->rowBlockSize(i) == camSize);
  }
  for (int i = 0; i < mat->numBlocksCol(); i++) {
    REQUIRE(mat->colBlockSize(i) == pointSizeVar[i]);
  }

  csBlockMatrix::SparsityPattern<Ordering> sp(numCams, numPoints);
  /*
  +-------------------+
  | X |   | X | X |   |
  +-------------------+
  |   | X |   |   | X |
  +-------------------+
  | X | X |   |   | X |
  */
  sp.add(0, 0);
  sp.add(0, 2);
  sp.add(0, 3);
  sp.add(1, 1);
  sp.add(1, 4);
  sp.add(2, 0);
  sp.add(2, 1);
  sp.add(2, 4);

  csBlockMatrix::SparsityPatternBlockDescriptor<Ordering> spbd(sp, *mat);

  if (Ordering == csBlockMatrix::ColMajor) {
    REQUIRE(sp.outerSize() == numPoints);
    REQUIRE(spbd.outerSize() == numPoints);

    REQUIRE(spbd.stride(0) == mat->rowBlockSize(0) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(0, 0) == 0);
    REQUIRE(spbd.offset(0, 1) == mat->rowBlockSize(0));

    int numEl = mat->colBlockSize(0) * (mat->rowBlockSize(0) + mat->rowBlockSize(2));

    REQUIRE(spbd.stride(1) == mat->rowBlockSize(1) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(1, 0) == numEl);
    REQUIRE(spbd.offset(1, 1) == numEl + mat->rowBlockSize(1));
    numEl += mat->colBlockSize(1) * (mat->rowBlockSize(1) + mat->rowBlockSize(2));

    REQUIRE(spbd.stride(2) == mat->rowBlockSize(0));
    REQUIRE(spbd.offset(2, 0) == numEl);
    numEl += mat->colBlockSize(2) * mat->rowBlockSize(0);

    REQUIRE(spbd.stride(3) == mat->rowBlockSize(0));
    REQUIRE(spbd.offset(3, 0) == numEl);
    numEl += mat->colBlockSize(3) * mat->rowBlockSize(0);

    REQUIRE(spbd.stride(4) == mat->rowBlockSize(1) + mat->rowBlockSize(2));
    REQUIRE(spbd.offset(4, 0) == numEl);
    REQUIRE(spbd.offset(4, 1) == numEl + mat->rowBlockSize(1));
    numEl += mat->colBlockSize(4) * (mat->rowBlockSize(1) + mat->rowBlockSize(2));

    REQUIRE(numEl == spbd.nonZeroCoeffs());
  } else if (Ordering == csBlockMatrix::RowMajor) {
    REQUIRE(sp.outerSize() == numCams);
    REQUIRE(spbd.outerSize() == numCams);

    REQUIRE(spbd.stride(0) == mat->colBlockSize(0) + mat->colBlockSize(2) + mat->colBlockSize(3));

    REQUIRE(spbd.offset(0, 0) == 0);
    REQUIRE(spbd.offset(0, 1) == mat->colBlockSize(0));
    REQUIRE(spbd.offset(0, 2) == mat->colBlockSize(0) + mat->colBlockSize(2));

    int numEl = mat->rowBlockSize(0) * (mat->colBlockSize(0) + mat->colBlockSize(2) + mat->colBlockSize(3));

    REQUIRE(spbd.stride(1) == mat->colBlockSize(1) + mat->colBlockSize(4));
    REQUIRE(spbd.offset(1, 0) == numEl);
    REQUIRE(spbd.offset(1, 1) == numEl + mat->colBlockSize(1));
    numEl += mat->rowBlockSize(1) * (mat->colBlockSize(1) + mat->colBlockSize(4));

    REQUIRE(spbd.stride(2) == mat->colBlockSize(0) + mat->colBlockSize(1) + mat->colBlockSize(4));
    REQUIRE(spbd.offset(2, 0) == numEl);
    REQUIRE(spbd.offset(2, 1) == numEl + mat->colBlockSize(0));
    REQUIRE(spbd.offset(2, 2) == numEl + mat->colBlockSize(0) + mat->colBlockSize(1));
    numEl += mat->rowBlockSize(2) * (mat->colBlockSize(0) + mat->colBlockSize(1) + mat->colBlockSize(4));

    REQUIRE(numEl == spbd.nonZeroCoeffs());
  }
}
