/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/21 20:53:44 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
  int cnt = 0;
  int max = 30;

  for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); it++) {
    cnt++;
    if (cnt <= max)
      os << *it << " ";
  }
  if (cnt > max)
    os << "... (" << cnt << " items)";
  return os;
}

bool PmergeMe::compare(int a, int b) {
  return (CountedInt(a) > CountedInt(b));
}

// ===================================================================
//   PmergeMe
// ===================================================================
//     constructor
PmergeMe::PmergeMe(std::vector<int> input)
    : input_(input), pool_v_(std::vector<Node>()),
      result_v_(std::vector<int>()), pool_d_(std::vector<Node>()),
      result_d_(std::vector<int>()) {}

//     destructor
PmergeMe::~PmergeMe() {}

//     accesser
const std::vector<int>& PmergeMe::getInput() const {
  return input_;
}
const std::vector<int>& PmergeMe::getResult_v() const {
  return result_v_;
}
const std::vector<int>& PmergeMe::getResult_d() const {
  return result_d_;
}

//     solver
// ============== vector ==============
void PmergeMe::solve_v() {
  CountedInt::resetCnt();
  pool_v_.reserve(input_.size() * 2);

  std::vector<int> ids;
  ids.reserve(input_.size());

  for (std::vector<int>::const_iterator it = input_.begin(); it != input_.end();
       it++) {
    pool_v_.push_back(Node(*it));
    ids.push_back(static_cast<int>(pool_v_.size() - 1));
  }
  TOUT << pool_v_;
  std::vector<int> result = solve_v(ids);

  result_v_.reserve(input_.size());
  for (std::vector<int>::const_iterator it = result.begin();
       it != result.begin() + input_.size(); it++)
    result_v_.push_back(pool_v_[*it].val);

  DOUT << "[debug] compare count: " << CountedInt::getCnt() << std::endl;
}

std::vector<int> PmergeMe::solve_v(std::vector<int>& v) {
  int size = v.size() - hasRemainder(v);

  if (size <= 2) {
    std::vector<int> ret(v);
    if (size == 2 && compare(pool_v_[v[0]].val, pool_v_[v[1]].val)) {
      ret[0] = v[1];
      ret[1] = v[0];
    }
    putResult(TOUT, pool_v_, v);
    return ret;
  }
  std::vector<int> folded = fold_v(v);
  std::vector<int> solved = solve_v(folded);
  return expand_v(solved);
}

std::vector<int> PmergeMe::fold_v(std::vector<int>& v) {
  int size = v.size() - hasRemainder(v);
  std::vector<int> ret;
  ret.reserve(v.size() / 2 + 1);
  int level = pool_v_[v[0]].level + 1;

  for (int i = 0; i + 1 < size; i += 2) {
    if (compare(pool_v_[v[i]].val, pool_v_[v[i + 1]].val))
      pool_v_.push_back(Node(level, pool_v_[v[i]].val, v[i], v[i + 1]));
    else
      pool_v_.push_back(Node(level, pool_v_[v[i + 1]].val, v[i + 1], v[i]));
    ret.push_back(pool_v_.size() - 1);
  }
  Node rem = Node(level, 0, -1, -1);
  if (size % 2)
    rem.highIdx = v[size - 1];
  if (hasRemainder(v))
    rem.lowIdx = v[size];
  if (rem.hasHigh() || rem.hasLow()) {
    pool_v_.push_back(rem);
    ret.push_back(pool_v_.size() - 1);
  }
  putResult(TOUT, pool_v_, v);
  return ret;
}

std::vector<int> PmergeMe::expand_v(std::vector<int>& v) {
  std::vector<int> ret;
  ret.reserve(v.size() * 2 + 1);
  size_t n = 4;
  std::vector<int>::const_iterator processed = v.begin();

  ret.push_back(pool_v_[v[0]].lowIdx);
  for (std::vector<int>::const_iterator it = v.begin();
       it != v.end() - hasRemainder(v); it++) {
    if (pool_v_[*it].hasHigh())
      ret.push_back(pool_v_[*it].highIdx);
    if (ret.size() >= n) {
      processLowElems(ret, it, processed, n);
      processed = it;
      n *= 2;
    }
    if (it == v.end() - hasRemainder(v) - 1) {
      if (hasRemainder(v) && pool_v_[v.back()].hasHigh()) {
        binaryInsert(ret, ret.begin(), ret.end() + 1,
                     pool_v_[v.back()].highIdx);
      }
      processLowElems(ret, it, processed, n);
      processed = it;
    }
  }
  if (hasRemainder(v) && pool_v_[v.back()].hasLow())
    ret.push_back(pool_v_[v.back()].lowIdx);
  return ret;
}

void PmergeMe::processLowElems(std::vector<int>& ret,
                               std::vector<int>::const_iterator begin,
                               std::vector<int>::const_iterator end, size_t n) {
  for (std::vector<int>::const_iterator it = begin; it != end; it--) {
    PmergeMe::binaryInsert(ret, ret.begin(),
                           ret.begin() + std::min(n, ret.size()),
                           pool_v_[*it].lowIdx);
  }
}

bool PmergeMe::hasRemainder(const std::vector<int>& v) {
  Node node = pool_v_[v.back()];

  while (true) {
    if (!node.hasHigh() && !node.hasLow())
      return false;
    if (!node.hasHigh() || !node.hasLow())
      return true;
    node = pool_v_[node.lowIdx];
  }
}

std::vector<int>::iterator
PmergeMe::binaryInsert(std::vector<int>& v, std::vector<int>::iterator begin,
                       std::vector<int>::iterator end, int idx) {
  int index = getIndexToInsert(begin, end, idx);
  return v.insert(begin + index, idx);
}

int PmergeMe::getIndexToInsert(std::vector<int>::const_iterator begin,
                               std::vector<int>::const_iterator end, int idx) {
  int left = -1;
  int right = end - begin - 1;
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
    if (!compare(pool_v_[idx].val,
                 pool_v_[*(begin + mid)].val)) // if(node<=*(begin + mid))
      right = mid;
    else
      left = mid;
  }
  return right;
}

// ============== deque ==============
void PmergeMe::solve_d() {
  CountedInt::resetCnt();
  pool_d_.reserve(input_.size() * 2);

  std::deque<int> ids;

  for (std::vector<int>::const_iterator it = input_.begin(); it != input_.end();
       it++) {
    pool_d_.push_back(Node(*it));
    ids.push_back(static_cast<int>(pool_d_.size() - 1));
  }
  TOUT << pool_d_;
  std::deque<int> result = solve_d(ids);

  result_d_.reserve(input_.size());
  for (std::deque<int>::const_iterator it = result.begin(); it != result.end();
       it++)
    result_d_.push_back(pool_d_[*it].val);
  DOUT << "[debug] compare count: " << CountedInt::getCnt() << std::endl;
}

std::deque<int> PmergeMe::solve_d(std::deque<int>& v) {
  int size = v.size() - hasRemainder(v);

  if (size <= 2) {
    std::deque<int> ret(v);
    if (size == 2 && compare(pool_d_[v[0]].val, pool_d_[v[1]].val)) {
      ret[0] = v[1];
      ret[1] = v[0];
    }
    return ret;
  }
  std::deque<int> folded = fold_d(v);
  std::deque<int> solved = solve_d(folded);
  return expand_d(solved);
}

std::deque<int> PmergeMe::fold_d(std::deque<int>& v) {
  int size = v.size() - hasRemainder(v);
  std::deque<int> ret;
  int level = pool_d_[v[0]].level + 1;

  for (int i = 0; i + 1 < size; i += 2) {
    if (compare(pool_d_[v[i]].val, pool_d_[v[i + 1]].val))
      pool_d_.push_back(Node(level, pool_d_[v[i]].val, v[i], v[i + 1]));
    else
      pool_d_.push_back(Node(level, pool_d_[v[i + 1]].val, v[i + 1], v[i]));
    ret.push_back(pool_d_.size() - 1);
  }
  Node rem = Node(level, 0, -1, -1);
  if (size % 2)
    rem.highIdx = v[size - 1];
  if (hasRemainder(v))
    rem.lowIdx = v[size];
  if (rem.hasHigh() || rem.hasLow()) {
    pool_d_.push_back(rem);
    ret.push_back(pool_d_.size() - 1);
  }
  return ret;
}

std::deque<int> PmergeMe::expand_d(std::deque<int>& v) {
  std::deque<int> ret;
  size_t n = 4;
  std::deque<int>::const_iterator processed = v.begin();

  ret.push_back(pool_d_[v[0]].lowIdx);
  for (std::deque<int>::const_iterator it = v.begin();
       it != v.end() - hasRemainder(v); it++) {
    if (pool_d_[*it].hasHigh())
      ret.push_back(pool_d_[*it].highIdx);
    if (ret.size() >= n) {
      processLowElems(ret, it, processed, n);
      processed = it;
      n *= 2;
    }
    if (it == v.end() - hasRemainder(v) - 1) {
      if (hasRemainder(v) && pool_d_[v.back()].hasHigh()) {
        binaryInsert(ret, ret.begin(), ret.end() + 1,
                     pool_d_[v.back()].highIdx);
      }
      processLowElems(ret, it, processed, n);
      processed = it;
    }
  }
  if (hasRemainder(v) && pool_d_[v.back()].hasLow())
    ret.push_back(pool_d_[v.back()].lowIdx);
  return ret;
}

void PmergeMe::processLowElems(std::deque<int>& ret,
                               std::deque<int>::const_iterator begin,
                               std::deque<int>::const_iterator end, size_t n) {
  for (std::deque<int>::const_iterator it = begin; it != end; it--) {
    PmergeMe::binaryInsert(ret, ret.begin(),
                           ret.begin() + std::min(n, ret.size()),
                           pool_d_[*it].lowIdx);
  }
}

bool PmergeMe::hasRemainder(const std::deque<int>& v) {
  Node node = pool_d_[v.back()];

  while (true) {
    if (!node.hasHigh() && !node.hasLow())
      return false;
    if (!node.hasHigh() || !node.hasLow())
      return true;
    node = pool_d_[node.lowIdx];
  }
}

std::deque<int>::iterator
PmergeMe::binaryInsert(std::deque<int>& v, std::deque<int>::iterator begin,
                       std::deque<int>::iterator end, int idx) {
  int index = getIndexToInsert(begin, end, idx);
  return v.insert(begin + index, idx);
}

int PmergeMe::getIndexToInsert(std::deque<int>::const_iterator begin,
                               std::deque<int>::const_iterator end, int idx) {
  int left = -1;
  int right = end - begin - 1;
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
    if (!compare(pool_d_[idx].val,
                 pool_d_[*(begin + mid)].val)) // if(node<=*(begin + mid))
      right = mid;
    else
      left = mid;
  }
  return right;
}

// printer
std::ostream& operator<<(std::ostream& os, const class PmergeMe& pmm) {
  os << "input: " << pmm.getInput();
  os << "result(vector): " << pmm.getResult_v();
  os << "result(deque): " << pmm.getResult_d();
  return os;
}

// ===================================================================
//   Node
// ===================================================================
//     constructor
Node::Node() : level(0), val(0), highIdx(-1), lowIdx(-1) {}
Node::Node(int v) : level(0), val(v), highIdx(-1), lowIdx(-1) {}

Node::Node(int lvl, int v, int hi, int low)
    : level(lvl), val(v), highIdx(hi), lowIdx(low) {}

bool Node::hasHigh() const {
  return highIdx != -1;
}
bool Node::hasLow() const {
  return lowIdx != -1;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Node>& pool) {
  Node node;
  for (size_t i = 0; i < pool.size(); i++)
    os << "pool[" << i << "]: {" << pool[i].level << ", " << pool[i].val << ", "
       << pool[i].highIdx << ", " << pool[i].lowIdx << "}, ";
  os << std::endl;
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::deque<Node>& pool) {
  Node node;
  for (size_t i = 0; i < pool.size(); i++)
    os << "pool[" << i << "]: {" << pool[i].level << ", " << pool[i].val << ", "
       << pool[i].highIdx << ", " << pool[i].lowIdx << "}, ";
  os << std::endl;
  return os;
}

// ===================================================================
//   CountedInt
// ===================================================================
//     constructor
int CountedInt::cnt_ = 0;

CountedInt::CountedInt(int v) : v_(v) {}
CountedInt::CountedInt(const CountedInt& other) {
  v_ = other.v_;
}

//     destructor
CountedInt::~CountedInt() {}

CountedInt& CountedInt::operator=(const CountedInt& other) {
  if (this == &other)
    return *this;
  v_ = other.v_;
  return *this;
}

//     accesser
int CountedInt::getValue() const {
  return v_;
}
int CountedInt::getCnt() {
  return cnt_;
}
void CountedInt::resetCnt() {
  cnt_ = 0;
}

bool CountedInt::operator>(const CountedInt& rhs) const {
  ++cnt_;
  TOUT << "[debug] cnt: " << cnt_ << ", compare: " << v_ << " > " << rhs.v_
       << std::endl;
  return v_ > rhs.v_;
}
