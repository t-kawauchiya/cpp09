/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/07 13:33:45 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include "CountedInt.hpp"
#include <cctype>
#include <iostream>
#include <vector>

// PmergeMe
//   constructor
PmergeMe::PmergeMe() : data_() {}

PmergeMe::PmergeMe(std::vector<Node> data) : data_(data) {}

PmergeMe::PmergeMe(std::vector<int> input) {
  for (std::vector<int>::const_iterator it = input.begin(); it != input.end();
       it++)
    data_.push_back(Node(*it));
}

PmergeMe::PmergeMe(const PmergeMe& other) {
  data_ = other.data_;
}

//   destructor
PmergeMe::~PmergeMe() {}

//   operator
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
  if (this == &other)
    return *this;
  data_ = other.data_;
  return *this;
}

//   accesser
std::vector<Node> PmergeMe::getData() const {
  return data_;
}

std::vector<int> PmergeMe::getResult() const {
  std::vector<int> res;
  for (std::vector<Node>::const_iterator it = data_.begin(); it != data_.end();
       it++) {
    res.push_back(it->getVal());
  }
  return res;
}

//   solver
void PmergeMe::solve() {
  std::cout << "input: " << data_ << std::endl;
  data_ = solve(data_);
}

std::vector<Node> PmergeMe::solve(const std::vector<Node>& v) {
  int size = v.size() - has_remainder(v);

  if (size <= 2) {
    std::cout << "\n===== size<=2 =====\n";
    std::vector<Node> ret(v);
    if (size == 2 && compare(v[0], v[1]))
      PmergeMe::swap(ret[0], ret[1]);
    return ret;
  }
  std::vector<Node> folded = fold(v);
  std::vector<Node> solved = solve(folded);
  return expand(solved);
}

void PmergeMe::swap(Node& a, Node& b) {
  Node c(a);
  a = b;
  b = c;
}

bool PmergeMe::compare(const Node& a, const Node& b) {
  // return (a.getIval() > b.getIval());
  return (CountedInt(a.getTopVal()) > CountedInt(b.getTopVal()));
}

bool PmergeMe::has_remainder(std::vector<Node> v) {
  Node* node = &v.back();
  int lvl = node->getLevel();

  while (lvl--) {
    if (!node->getLow() || !node->getLow())
      return true;
    node = node->getLow();
  }
  return false;
}

std::vector<Node> PmergeMe::fold(std::vector<Node> v) {
  std::vector<Node> ret;
  int size = v.size() - has_remainder(v);
  int level = v[0].getLevel() + 1;

  std::cout << "=====fold=====" << std::endl;
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

  std::cout << "\nsize: " << ret.size() - has_remainder(ret)
            << ", has_remainder: " << has_remainder(ret) << "\n"
            << ret;
  return ret;
}

std::vector<Node> PmergeMe::expand(const std::vector<Node> v) {
  std::vector<Node> ret;

  std::cout << "\n=====expand=====\n";
  size_t n = 4;
  ret.push_back(*v.begin()->getLow());
  std::cout << "push begin.low: \n" << ret << "\n--------------" << std::endl;
  std::vector<Node>::const_iterator processed = v.begin();
  for (std::vector<Node>::const_iterator it = v.begin();
       it + has_remainder(v) != v.end(); it++) {
    if (it->getHigh())
      ret.push_back(*it->getHigh());
    std::cout << "push *it->high: \n" << ret << "\n--------------" << std::endl;
    if (has_remainder(v) && it == v.end() - 2 && v.back().getHigh()) {
      std::cout << "insert remainder: \n";
      int index = PmergeMe::getIndexToInsert(ret.begin(), ret.end(),
                                             *v.back().getHigh());
      ret.insert(ret.begin() + index, *v.back().getHigh());
      std::cout << ret << "\n--------------" << std::endl;
    }
    if (ret.size() >= n) {
      for (std::vector<Node>::const_iterator it2 = it;
           it2 != processed && it2->getLow(); it2--) {
        std::cout << "insert *it2->low(high: " << it2->getHigh()->getTopVal()
                  << "): \n";
        int index = PmergeMe::getIndexToInsert(ret.begin(), ret.begin() + n - 1,
                                               *it2->getLow());
        ret.insert(ret.begin() + index, *it2->getLow());
        std::cout << ret << "\n--------------" << std::endl;
      }
      n *= 2;
      processed = it;
    }
  }
  if (has_remainder(v) && v.back().getLow()) {
    ret.push_back(*v.back().getLow());
    std::cout << "update remainder: \n" << ret << "--------------" << std::endl;
  }
  return ret;
}

int PmergeMe::getIndexToInsert(std::vector<Node>::const_iterator begin,
                               std::vector<Node>::const_iterator end,
                               Node node) {
  int left = -1;
  int right = end - begin; // check
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
    if (compare(*(begin + mid), node))
      right = mid;
    else
      left = mid;
  }
  return right;
}

//   printer
std::ostream& operator<<(std::ostream& os, const PmergeMe& pmm) {
  const std::vector<Node> data = pmm.getData();
  os << data;
  return os;
}

// Node
//   constructor
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

//   destructor
Node::~Node() {
  delete this->high_;
  delete this->low_;
}

//   operator
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

//   accesser
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

//   printer
std::ostream& operator<<(std::ostream& os, const std::vector<Node>& v) {
  for (std::vector<Node>::const_iterator it = v.begin(); it != v.end(); it++) {
    if (PmergeMe::has_remainder(v) && it == v.end() - 1 &&
        v.begin()->getLevel() > 1)
      os << "---rem---\n";
    os << *it;
  }
  return os;
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
