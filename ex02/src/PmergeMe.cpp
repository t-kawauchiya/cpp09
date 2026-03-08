/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/08 10:37:30 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
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
//   PmergeMeVector
// ===================================================================
//     constructor
PmergeMeVector::PmergeMeVector() : data_() {}

PmergeMeVector::PmergeMeVector(std::vector<Node> data) : data_(data) {}

PmergeMeVector::PmergeMeVector(std::vector<int> input) {
  for (std::vector<int>::const_iterator it = input.begin(); it != input.end();
       it++)
    data_.push_back(Node(*it));
}

PmergeMeVector::PmergeMeVector(const PmergeMeVector& other) {
  data_ = other.data_;
}

//     destructor
PmergeMeVector::~PmergeMeVector() {}

//     operator
PmergeMeVector& PmergeMeVector::operator=(const PmergeMeVector& other) {
  if (this == &other)
    return *this;
  data_ = other.data_;
  return *this;
}

//     accesser
std::vector<Node> PmergeMeVector::getData() const {
  return data_;
}

std::vector<int> PmergeMeVector::getResult() const {
  std::vector<int> res;
  for (std::vector<Node>::const_iterator it = data_.begin(); it != data_.end();
       it++) {
    res.push_back(it->getVal());
  }
  return res;
}

//     solver
void PmergeMeVector::solve() {
  data_ = solve(data_);
  DOUT << "[debug] compare count: " << CountedInt::getCnt() << std::endl;
}

std::vector<Node> PmergeMeVector::solve(const std::vector<Node>& v) {
  int size = v.size() - has_remainder(v);

  if (size <= 2) {
    // DOUT << "\n===== size<=2 =====\n";
    std::vector<Node> ret(v);
    if (size == 2 && compare(v[0], v[1]))
      swap(ret[0], ret[1]);
    // DOUT << ret;
    return ret;
  }
  std::vector<Node> folded = fold(v);
  std::vector<Node> solved = solve(folded);
  return expand(solved);
}

bool PmergeMeVector::has_remainder(std::vector<Node> v) {
  Node* node = &v.back();
  int lvl = node->getLevel();

  while (lvl--) {
    if (!node->getLow() || !node->getLow())
      return true;
    node = node->getLow();
  }
  return false;
}

std::vector<Node> PmergeMeVector::fold(std::vector<Node> v) {
  std::vector<Node> ret;
  int size = v.size() - has_remainder(v);
  int level = v[0].getLevel() + 1;

  // DOUT << "=====fold=====" << std::endl;
  for (int i = 0; i + 1 < size; i += 2) {
    if (compare(v[i], v[i + 1]))
      ret.push_back(Node(level, 0, v[i], v[i + 1]));
    else
      ret.push_back(Node(level, 0, v[i + 1], v[i]));
  }

  Node rem = Node(level, 0, NULL, NULL);
  if (size % 2)
    rem.setHigh(new Node(v[size - 1]));
  if (has_remainder(v))
    rem.setLow(new Node(v[size]));
  if (rem.getHigh() || rem.getLow())
    ret.push_back(rem);

  // DOUT << "\nsize: " << ret.size() - has_remainder(ret)
  //      << ", has_remainder: " << has_remainder(ret) << "\n"
  //      << ret;
  return ret;
}

std::vector<Node> PmergeMeVector::expand(const std::vector<Node> v) {
  // DOUT << "\n=====expand=====\n";
  std::vector<Node> ret;
  size_t n = 4;
  std::vector<Node>::const_iterator processed = v.begin();

  ret.push_back(*v.begin()->getLow());
  // DOUT << "push begin-low: \n" << ret << separator << std::endl;

  for (std::vector<Node>::const_iterator it = v.begin();
       it != v.end() - has_remainder(v); it++) {
    if (it->getHigh())
      ret.push_back(*it->getHigh());
    // DOUT << "push high: \n" << ret << separator << std::endl;
    if (ret.size() >= n) {
      for (std::vector<Node>::const_iterator it2 = it;
           it2 != processed && it2->getLow(); it2--) {
        binaryInsert(ret, ret.begin(), ret.begin() + n, *it2->getLow());
        // DOUT << "insert low(high: " << it2->getHigh()->getTopVal() << "):\n"
        //      << ret << separator << std::endl;
      }
      n *= 2;
      processed = it;
    }
    if (it == v.end() - has_remainder(v) - 1) {
      if (has_remainder(v) && v.back().getHigh()) {
        binaryInsert(ret, ret.begin(), ret.end() + 1, *v.back().getHigh());
        // DOUT << "insert remainder: \n" << ret << separator << std::endl;
      }
      for (std::vector<Node>::const_iterator it2 = it;
           it2 != processed && it2->getLow(); it2--) {
        binaryInsert(ret, ret.begin(), ret.begin() + std::min(n, ret.size()),
                     *it2->getLow());
        // DOUT << "insert low(high: " << it2->getHigh()->getTopVal() << "):\n"
        //      << ret << separator << std::endl;
      }
      processed = it;
    }
  }

  if (has_remainder(v) && v.back().getLow())
    ret.push_back(*v.back().getLow());
  // DOUT << "update remainder: \n" << ret << separator << std::endl;
  return ret;
}

std::vector<Node>::iterator PmergeMeVector::binaryInsert(
    std::vector<Node>& v, std::vector<Node>::iterator first,
    std::vector<Node>::iterator end, const Node& item) {
  int index = PmergeMeVector::getIndexToInsert(first, end, item);
  return v.insert(first + index, item);
}

int PmergeMeVector::getIndexToInsert(std::vector<Node>::const_iterator begin,
                                     std::vector<Node>::const_iterator end,
                                     Node node) {
  // DOUT << "getIndexToInsert" << std::endl;
  int left = -1;
  int right = end - begin - 1;
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
    // DOUT << "mid: " << mid << ", left: " << left << ", right: " << right
    //      << std::endl;
    if (!compare(node, *(begin + mid))) // node <= *(begin + mid)
      right = mid;
    else
      left = mid;
  }
  return right;
}

//     printer
std::ostream& operator<<(std::ostream& os, const PmergeMeVector& pmm) {
  const std::vector<Node> data = pmm.getData();
  os << data;
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Node>& v) {
  for (std::vector<Node>::const_iterator it = v.begin(); it != v.end(); it++) {
    if (PmergeMeVector::has_remainder(v) && it == v.end() - 1 &&
        v.begin()->getLevel() > 1)
      os << "---rem---\n";
    os << *it;
  }
  return os;
}

// ===================================================================
//   PmergeMeDeque
// ===================================================================
//     constructor
PmergeMeDeque::PmergeMeDeque() : data_() {}

PmergeMeDeque::PmergeMeDeque(std::deque<Node> data) : data_(data) {}

PmergeMeDeque::PmergeMeDeque(std::vector<int> input) {
  for (std::vector<int>::const_iterator it = input.begin(); it != input.end();
       it++)
    data_.push_back(Node(*it));
}

PmergeMeDeque::PmergeMeDeque(const PmergeMeDeque& other) {
  data_ = other.data_;
}

//     destructor
PmergeMeDeque::~PmergeMeDeque() {}

//     operator
PmergeMeDeque& PmergeMeDeque::operator=(const PmergeMeDeque& other) {
  if (this == &other)
    return *this;
  data_ = other.data_;
  return *this;
}

//     accesser
std::deque<Node> PmergeMeDeque::getData() const {
  return data_;
}

std::vector<int> PmergeMeDeque::getResult() const {
  std::vector<int> res;
  for (std::deque<Node>::const_iterator it = data_.begin(); it != data_.end();
       it++) {
    res.push_back(it->getVal());
  }
  return res;
}

//     solver
void PmergeMeDeque::solve() {
  data_ = solve(data_);
  // DOUT << "[debug] compare count: " << CountedInt::getCnt() << std::endl;
}

std::deque<Node> PmergeMeDeque::solve(const std::deque<Node>& v) {
  int size = v.size() - has_remainder(v);

  if (size <= 2) {
    // DOUT << "\n===== size<=2 =====\n";
    std::deque<Node> ret(v);
    if (size == 2 && compare(v[0], v[1]))
      swap(ret[0], ret[1]);
    // DOUT << ret;
    return ret;
  }
  std::deque<Node> folded = fold(v);
  std::deque<Node> solved = solve(folded);
  return expand(solved);
}

bool PmergeMeDeque::has_remainder(std::deque<Node> v) {
  Node* node = &v.back();
  int lvl = node->getLevel();

  while (lvl--) {
    if (!node->getLow() || !node->getLow())
      return true;
    node = node->getLow();
  }
  return false;
}

std::deque<Node> PmergeMeDeque::fold(std::deque<Node> v) {
  std::deque<Node> ret;
  int size = v.size() - has_remainder(v);
  int level = v[0].getLevel() + 1;

  // DOUT << "=====fold=====" << std::endl;
  for (int i = 0; i + 1 < size; i += 2) {
    if (compare(v[i], v[i + 1]))
      ret.push_back(Node(level, 0, v[i], v[i + 1]));
    else
      ret.push_back(Node(level, 0, v[i + 1], v[i]));
  }

  Node rem = Node(level, 0, NULL, NULL);
  if (size % 2)
    rem.setHigh(new Node(v[size - 1]));
  if (has_remainder(v))
    rem.setLow(new Node(v[size]));
  if (rem.getHigh() || rem.getLow())
    ret.push_back(rem);

  // DOUT << "\nsize: " << ret.size() - has_remainder(ret)
  //      << ", has_remainder: " << has_remainder(ret) << "\n"
  //      << ret;
  return ret;
}

std::deque<Node> PmergeMeDeque::expand(const std::deque<Node> v) {
  // DOUT << "\n=====expand=====\n";
  std::deque<Node> ret;
  size_t n = 4;
  std::deque<Node>::const_iterator processed = v.begin();

  ret.push_back(*v.begin()->getLow());
  // DOUT << "push begin-low: \n" << ret << separator << std::endl;

  for (std::deque<Node>::const_iterator it = v.begin();
       it != v.end() - has_remainder(v); it++) {
    if (it->getHigh())
      ret.push_back(*it->getHigh());
    // DOUT << "push high: \n" << ret << separator << std::endl;
    if (ret.size() >= n) {
      for (std::deque<Node>::const_iterator it2 = it;
           it2 != processed && it2->getLow(); it2--) {
        binaryInsert(ret, ret.begin(), ret.begin() + n, *it2->getLow());
        // DOUT << "insert low(high: " << it2->getHigh()->getTopVal() << "):\n"
        //      << ret << separator << std::endl;
      }
      n *= 2;
      processed = it;
    }
    if (it == v.end() - has_remainder(v) - 1) {
      if (has_remainder(v) && v.back().getHigh()) {
        binaryInsert(ret, ret.begin(), ret.end() + 1, *v.back().getHigh());
        // DOUT << "insert remainder: \n" << ret << separator << std::endl;
      }
      for (std::deque<Node>::const_iterator it2 = it;
           it2 != processed && it2->getLow(); it2--) {
        binaryInsert(ret, ret.begin(), ret.begin() + std::min(n, ret.size()),
                     *it2->getLow());
        // DOUT << "insert low(high: " << it2->getHigh()->getTopVal() << "):\n"
        //      << ret << separator << std::endl;
      }
      processed = it;
    }
  }
  if (has_remainder(v) && v.back().getLow())
    ret.push_back(*v.back().getLow());
  // DOUT << "update remainder: \n" << ret << separator << std::endl;
  return ret;
}

std::deque<Node>::iterator
PmergeMeDeque::binaryInsert(std::deque<Node>& v,
                            std::deque<Node>::iterator first,
                            std::deque<Node>::iterator end, const Node& item) {
  int index = PmergeMeDeque::getIndexToInsert(first, end, item);
  return v.insert(first + index, item);
}

int PmergeMeDeque::getIndexToInsert(std::deque<Node>::const_iterator begin,
                                    std::deque<Node>::const_iterator end,
                                    Node node) {
  int left = -1;
  int right = end - begin - 1;
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
    if (!compare(node, *(begin + mid))) // node <= *(begin + mid)
      right = mid;
    else
      left = mid;
  }
  return right;
}

//     printer
std::ostream& operator<<(std::ostream& os, const PmergeMeDeque& pmm) {
  const std::deque<Node> data = pmm.getData();
  os << data;
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::deque<Node>& v) {
  for (std::deque<Node>::const_iterator it = v.begin(); it != v.end(); it++) {
    if (PmergeMeDeque::has_remainder(v) && it == v.end() - 1 &&
        v.begin()->getLevel() > 1)
      os << "---rem---\n";
    os << *it;
  }
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

Node::Node(int lvl, int ival, const Node& a, const Node& b)
    : level_(lvl), val_(ival), high_(new Node(a)), low_(new Node(b)) {}

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
  delete low_;
  if (other.high_) {
    high_ = new Node(*other.high_);
  } else
    high_ = NULL;
  if (other.low_)
    low_ = new Node(*other.low_);
  else
    low_ = NULL;
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

std::ostream& operator<<(std::ostream& os, const Node& node) {
  int indent = 2 * (4 - node.getLevel());
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
//     printer
bool CountedInt::operator>(const CountedInt& rhs) const {
  ++cnt_;
  // DOUT << "[debug] cnt: " << cnt_ << ", compare: " << v_ << " > " << rhs.v_
  //      << std::endl;
  return v_ > rhs.v_;
}
