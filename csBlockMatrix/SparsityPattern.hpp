#pragma once
#include <csBlockMatrix/Global.hpp>

#include <cassert>
#include <set>
#include <vector>

#include <memory>

#include <Eigen/Sparse>

namespace csBlockMatrix {

template <int Ordering> class SparsityPattern {
  std::vector<std::set<int>> _sp;
  int _innerSize;
  int _count;

public:
  using SPtr = std::shared_ptr<SparsityPattern>;
  using CSPtr = std::shared_ptr<const SparsityPattern>;

  SparsityPattern(int nr, int nc);
  virtual ~SparsityPattern();

  static typename SparsityPattern::CSPtr makeDiagonal(int nr, int nc);

  void clear();

  void setDiagonal() {
    for (int i = 0; i < std::min(int(_sp.size()), _innerSize); i++) {
      auto ret = _sp[i].insert(i);
      if (ret.second)
        _count++;
    }
  }

  void add(int i, int j) {
    if (Ordering == csBlockMatrix::ColMajor) {
      assert(i < _innerSize);
      auto ret = _sp[j].insert(i);
      if (ret.second)
        _count++;
    } else if (Ordering == csBlockMatrix::RowMajor) {
      assert(j < _innerSize);
      auto ret = _sp[i].insert(j);
      if (ret.second)
        _count++;
    } else {
      __MAT_ASSERT_FALSE();
    }
  }

  bool has(int i, int j) const {
    if (Ordering == csBlockMatrix::ColMajor) {
      return _sp[j].find(i) != _sp[j].end();
    } else if (Ordering == csBlockMatrix::RowMajor) {
      return _sp[i].find(j) != _sp[i].end();
    } else {
      __MAT_ASSERT_FALSE();
    }
  }

  int count() const { return _count; }

  // if rowmajor, outer is "i", return is the list of occupied cols
  // if colmajor, outer is "j", return is the list of occupied rows
  const std::set<int> &inner(int outer) const { return _sp[outer]; }

  int outerSize() const { return _sp.size(); }

  int innerSize() const { return _innerSize; }

  Eigen::SparseMatrix<int, Ordering> toSparseMatrix() const;
};

using SparsityPatternColMajor = SparsityPattern<csBlockMatrix::ColMajor>;
using SparsityPatternRowMajor = SparsityPattern<csBlockMatrix::RowMajor>;

} // namespace csBlockMatrix
