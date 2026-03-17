/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/17 21:50:07 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

void swap(Node& a, Node& b) {
  Node c(a);
  a = b;
  b = c;
}

bool compare(const Node& a, const Node& b) {
  return (CountedInt(a.getTopVal()) > CountedInt(b.getTopVal()));
}

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

// ===================================================================
//   PmergeMe
// ===================================================================
//     constructor
PmergeMe::PmergeMe()
    : input_(std::vector<int>()), resultVector_(std::vector<int>()),
      resultDeque_(std::vector<int>()) {}
PmergeMe::PmergeMe(std::vector<int> input)
    : input_(input), resultVector_(std::vector<int>()),
      resultDeque_(std::vector<int>()) {}
PmergeMe::PmergeMe(const PmergeMe& other)
    : resultVector_(std::vector<int>()), resultDeque_(std::vector<int>()) {
  input_ = other.input_;
}

//     destructor
PmergeMe::~PmergeMe() {}

//     operator
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
  if (this == &other)
    return *this;
  input_ = other.input_;
  resultVector_ = other.resultVector_;
  resultDeque_ = other.resultDeque_;
  return *this;
}

//     accesser
std::vector<int> PmergeMe::getInput() const {
  return input_;
}
std::vector<int> PmergeMe::getResultVector() const {
  return resultVector_;
}
std::vector<int> PmergeMe::getResultDeque() const {
  return resultDeque_;
}

//     solver
// ============== vector ==============
void PmergeMe::solveVector() {
  CountedInt::resetCnt();
  std::vector<Node> input_vector;
  for (std::vector<int>::const_iterator it = input_.begin(); it != input_.end();
       it++)
    input_vector.push_back(Node(*it));

  std::vector<Node> result_vector = solveVector(input_vector);

  for (std::vector<Node>::const_iterator it = result_vector.begin();
       it != result_vector.end(); it++)
    resultVector_.push_back(it->getVal());

  DOUT << "[debug] compare count: " << CountedInt::getCnt() << std::endl;
}

std::vector<Node> PmergeMe::solveVector(const std::vector<Node>& v) {
  int size = v.size() - hasRemainder(v);

  if (size <= 2) {
    // DOUT << "\n===== size<=2 =====\n";
    std::vector<Node> ret(v);
    if (size == 2 && compare(v[0], v[1]))
      swap(ret[0], ret[1]);
    // DOUT << ret;
    return ret;
  }
  std::vector<Node> folded = foldVector(v);
  std::vector<Node> solved = solveVector(folded);
  return expandVector(solved);
}

std::vector<Node> PmergeMe::foldVector(std::vector<Node> v) {
  std::vector<Node> ret;
  int size = v.size() - hasRemainder(v);
  int level = v[0].getLevel() + 1;

  // DOUT << "=====fold=====" << std::endl;
  for (int i = 0; i + 1 < size; i += 2) {
    if (compare(v[i], v[i + 1]))
      ret.push_back(Node(level, 0, &v[i], &v[i + 1]));
    else
      ret.push_back(Node(level, 0, &v[i + 1], &v[i]));
  }

  Node rem = Node(level, 0, NULL, NULL);
  if (size % 2)
    rem.setHigh(new Node(v[size - 1]));
  if (hasRemainder(v))
    rem.setLow(new Node(v[size]));
  if (rem.getHigh() || rem.getLow())
    ret.push_back(rem);

  // DOUT << "size: " << ret.size() - hasRemainder(ret)
  //      << ", hasRemainder: " << hasRemainder(ret) << "\n"
  // << ret;
  return ret;
}

std::vector<Node> PmergeMe::expandVector(const std::vector<Node> v) {
  // DOUT << "\n=====expand=====\n";
  std::vector<Node> ret;
  size_t n = 4;
  std::vector<Node>::const_iterator processed = v.begin();

  ret.push_back(*v.begin()->getLow());
  // DOUT << "push begin-low: " << v.begin()->getLow()->getTopVal() << "\n"
  //      << ret << separator << std::endl;

  for (std::vector<Node>::const_iterator it = v.begin();
       it != v.end() - hasRemainder(v); it++) {
    if (it->getHigh())
      ret.push_back(*it->getHigh());
    // DOUT << "push high: " << it->getHigh()->getTopVal() << "\n"
    //      << ret << separator << std::endl;
    if (ret.size() >= n) {
      processLowElems(ret, it, processed, n);
      processed = it;
      n *= 2;
    }
    if (it == v.end() - hasRemainder(v) - 1) {
      if (hasRemainder(v) && v.back().getHigh()) {
        binaryInsert(ret, ret.begin(), ret.end() + 1, *v.back().getHigh());
        // DOUT << "insert remainder: " << v.back().getHigh()->getTopVal() <<
        // "\n"
        //      << ret << separator << std::endl;
      }
      processLowElems(ret, it, processed, n);
      processed = it;
    }
  }
  if (hasRemainder(v) && v.back().getLow())
    ret.push_back(*v.back().getLow());
  // DOUT << "update remainder: \n" << ret << separator << std::endl;
  return ret;
}

void PmergeMe::processLowElems(std::vector<Node>& ret,
                               std::vector<Node>::const_iterator begin,
                               std::vector<Node>::const_iterator end,
                               size_t n) {
  for (std::vector<Node>::const_iterator it = begin; it != end && it->getLow();
       it--) {
    PmergeMe::binaryInsert(
        ret, ret.begin(), ret.begin() + std::min(n, ret.size()), *it->getLow());
    // DOUT << "insert low: " << it->getLow()->getTopVal()
    //      << " (high: " << it->getHigh()->getTopVal() << ")\n"
    //      << ret << separator << std::endl;
  }
}

bool PmergeMe::hasRemainder(std::vector<Node> v) {
  Node* node = &v.back();
  int lvl = node->getLevel();

  while (lvl--) {
    if (!node->getLow() || !node->getLow())
      return true;
    node = node->getLow();
  }
  return false;
}

std::vector<Node>::iterator
PmergeMe::binaryInsert(std::vector<Node>& v, std::vector<Node>::iterator begin,
                       std::vector<Node>::iterator end, const Node& item) {
  int index = PmergeMe::getIndexToInsert(begin, end, item);
  return v.insert(begin + index, item);
}

int PmergeMe::getIndexToInsert(std::vector<Node>::const_iterator begin,
                               std::vector<Node>::const_iterator end,
                               Node node) {
  int left = -1;
  int right = end - begin - 1;
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
    if (!compare(node, *(begin + mid)))
      right = mid;
    else
      left = mid;
  }
  return right;
}

// =================== deque ===================
void PmergeMe::solveDeque() {
  CountedInt::resetCnt();
  std::deque<Node> input_deque;
  for (std::vector<int>::const_iterator it = input_.begin(); it != input_.end();
       it++) {
    input_deque.push_back(Node(*it));
  }
  std::deque<Node> result_deque = solveDeque(input_deque);

  for (std::deque<Node>::const_iterator it = result_deque.begin();
       it != result_deque.end(); it++)
    resultDeque_.push_back(it->getVal());

  DOUT << "[debug] compare count: " << CountedInt::getCnt() << std::endl;
}

std::deque<Node> PmergeMe::solveDeque(const std::deque<Node>& v) {
  int size = v.size() - hasRemainder(v);

  if (size <= 2) {
    std::deque<Node> ret(v);
    if (size == 2 && compare(v[0], v[1]))
      swap(ret[0], ret[1]);
    return ret;
  }
  std::deque<Node> folded = foldDeque(v);
  std::deque<Node> solved = solveDeque(folded);
  return expandDeque(solved);
}

std::deque<Node> PmergeMe::foldDeque(std::deque<Node> v) {
  std::deque<Node> ret;
  int size = v.size() - hasRemainder(v);
  int level = v[0].getLevel() + 1;

  for (int i = 0; i + 1 < size; i += 2) {
    if (compare(v[i], v[i + 1]))
      ret.push_back(Node(level, 0, &v[i], &v[i + 1]));
    else
      ret.push_back(Node(level, 0, &v[i + 1], &v[i]));
  }

  Node rem = Node(level, 0, NULL, NULL);
  if (size % 2)
    rem.setHigh(new Node(v[size - 1]));
  if (hasRemainder(v))
    rem.setLow(new Node(v[size]));
  if (rem.getHigh() || rem.getLow())
    ret.push_back(rem);

  return ret;
}

std::deque<Node> PmergeMe::expandDeque(const std::deque<Node> v) {
  std::deque<Node> ret;
  size_t n = 4;
  std::deque<Node>::const_iterator processed = v.begin();

  ret.push_back(*v.begin()->getLow());

  for (std::deque<Node>::const_iterator it = v.begin();
       it != v.end() - hasRemainder(v); it++) {
    if (it->getHigh())
      ret.push_back(*it->getHigh());
    if (ret.size() >= n) {
      processLowElems(ret, it, processed, n);
      processed = it;
      n *= 2;
    }
    if (it == v.end() - hasRemainder(v) - 1) {
      if (hasRemainder(v) && v.back().getHigh())
        binaryInsert(ret, ret.begin(), ret.end() + 1, *v.back().getHigh());
      processLowElems(ret, it, processed, n);
      processed = it;
    }
  }
  if (hasRemainder(v) && v.back().getLow())
    ret.push_back(*v.back().getLow());
  return ret;
}

void PmergeMe::processLowElems(std::deque<Node>& ret,
                               std::deque<Node>::const_iterator begin,
                               std::deque<Node>::const_iterator end, size_t n) {
  for (std::deque<Node>::const_iterator it = begin; it != end && it->getLow();
       it--)
    PmergeMe::binaryInsert(
        ret, ret.begin(), ret.begin() + std::min(n, ret.size()), *it->getLow());
}

bool PmergeMe::hasRemainder(std::deque<Node> v) {
  Node* node = &v.back();
  int lvl = node->getLevel();

  while (lvl--) {
    if (!node->getLow() || !node->getLow())
      return true;
    node = node->getLow();
  }
  return false;
}

std::deque<Node>::iterator
PmergeMe::binaryInsert(std::deque<Node>& v, std::deque<Node>::iterator begin,
                       std::deque<Node>::iterator end, const Node& item) {
  int index = PmergeMe::getIndexToInsert(begin, end, item);
  return v.insert(begin + index, item);
}

int PmergeMe::getIndexToInsert(std::deque<Node>::const_iterator begin,
                               std::deque<Node>::const_iterator end,
                               Node node) {
  int left = -1;
  int right = end - begin - 1;
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
    if (!compare(node, *(begin + mid)))
      right = mid;
    else
      left = mid;
  }
  return right;
}

std::ostream& operator<<(std::ostream& os, const class PmergeMe& pmm) {
  os << "input: " << pmm.getInput();
  os << "result(vector): " << pmm.getResultVector();
  os << "result(deque): " << pmm.getResultDeque();
  return os;
}

// ===================================================================
//   Node
// ===================================================================
//     constructor
Node::Node(int val) : level_(0), val_(val), high_(NULL), low_(NULL) {}

Node::Node(int lvl, int ival, Node* high_p, Node* low_p)
    : level_(lvl), val_(ival) {
  high_ = high_p ? new Node(*high_p) : NULL;
  low_ = low_p ? (new Node(*low_p)) : NULL;
}

Node::Node(const Node& other) {
  level_ = other.level_;
  val_ = other.val_;
  if (other.high_)
    high_ = new Node(*other.high_);
  else
    high_ = NULL;
  if (other.low_)
    low_ = new Node(*other.low_);
  else
    low_ = NULL;
}

//     destructor
Node::~Node() {
  delete this->high_;
  delete this->low_;
}

//     operator
Node& Node::operator=(const Node& other) {
  if (this == &other)
    return *this;
  level_ = other.level_;
  val_ = other.val_;
  delete high_;
  high_ = other.high_ ? new Node(*other.high_) : NULL;
  delete low_;
  low_ = other.low_ ? new Node(*other.low_) : NULL;
  return *this;
}

//     accesser
int Node::getTopVal() const {
  int lvl = this->level_;
  const Node* node = this;
  while (lvl--) {
    if (!node->high_)
      return node->val_;
    node = node->high_;
  }
  return node->val_;
}
int Node::getLevel() const {
  return level_;
}
int Node::getVal() const {
  return val_;
}
Node* Node::getHigh() const {
  return high_;
}
Node* Node::getLow() const {
  return low_;
}
void Node::setHigh(Node* node) {
  high_ = node;
}
void Node::setLow(Node* node) {
  low_ = node;
}

//     printer
std::ostream& operator<<(std::ostream& os, const Node& node) {
  int indent = std::max(0, 2 * (4 - node.getLevel()));
  if (node.getLevel() == 0) {
    os << node.getVal() << " ";
    return os;
  }
  os << std::string(indent, ' ');
  os << "L" << node.getLevel();
  os << "{ ";
  if (node.getLevel() > 1)
    os << "\n";
  if (node.getHigh())
    os << *node.getHigh();
  else {
    if (node.getLevel() != 1)
      os << std::string(indent + 2, ' ');
    os << "NULL ";
    if (node.getLevel() > 1)
      os << "\n";
  }
  if (node.getLow())
    os << *node.getLow();
  else {
    if (node.getLevel() != 1)
      os << std::string(indent + 1, ' ');
    os << "NULL ";
    if (node.getLevel() > 1)
      os << "\n";
  }
  if (node.getLevel() != 1)
    os << std::string(indent, ' ');
  os << "}\n";
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Node>& v) {
  for (std::vector<Node>::const_iterator it = v.begin(); it != v.end(); it++) {
    if (PmergeMe::hasRemainder(v) && it == v.end() - 1 &&
        v.begin()->getLevel() > 1)
      os << "---rem---\n";
    os << *it;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::deque<Node>& v) {
  for (std::deque<Node>::const_iterator it = v.begin(); it != v.end(); it++) {
    if (PmergeMe::hasRemainder(v) && it == v.end() - 1 &&
        v.begin()->getLevel() > 1)
      os << "---rem---\n";
    os << *it;
  }
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
  // DOUT << "[debug] cnt: " << cnt_ << ", compare: " << v_ << " > " << rhs.v_
  //      << std::endl;
  return v_ > rhs.v_;
}
