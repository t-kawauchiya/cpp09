/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/06 23:45:53 by takawauc         ###   ########.fr       */
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

PmergeMe::PmergeMe(std::vector<t_node> data) : data_(data) {}

PmergeMe::PmergeMe(std::vector<int> input) {
  for (std::vector<int>::const_iterator it = input.begin(); it != input.end();
       it++)
    data_.push_back(t_node(*it));
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
std::vector<PmergeMe::t_node> PmergeMe::getData() const {
  return data_;
}

std::vector<int> PmergeMe::getResult() const {
  std::vector<int> res;
  for (std::vector<PmergeMe::t_node>::const_iterator it = data_.begin();
       it != data_.end(); it++) {
    res.push_back(it->i_val_);
  }
  return res;
}

//   solver
void PmergeMe::solve() {
  std::cout << "input: " << data_ << std::endl;
  data_ = solve(data_);
}

std::vector<PmergeMe::t_node>
PmergeMe::solve(const std::vector<PmergeMe::t_node>& v) {
  int size = v.size() - has_remainder(v);
  if (size == 2) {
    std::cout << "\n===== size==2 =====\n";
    std::vector<PmergeMe::t_node> ret(v);
    // if (ret[0].getIval() > ret[1].getIval())
    if (CountedInt(v[0].getIval()) > CountedInt(v[1].getIval()))
      PmergeMe::swap(ret[0], ret[1]);
    return ret;
  }
  std::vector<PmergeMe::t_node> folded = fold(v);
  std::vector<PmergeMe::t_node> solved = solve(folded);
  return expand(solved);
}

void PmergeMe::swap(PmergeMe::t_node& a, PmergeMe::t_node& b) {
  PmergeMe::t_node c(a);
  a = b;
  b = c;
}

bool PmergeMe::has_remainder(std::vector<PmergeMe::t_node> v) {
  PmergeMe::t_node* node = &v.back();
  int lvl = node->level_;
  while (lvl--) {
    if (!node->high_ || !node->low_)
      return true;
    node = node->low_;
  }
  return false;
}

std::vector<PmergeMe::t_node> PmergeMe::fold(std::vector<PmergeMe::t_node> v) {

  std::vector<PmergeMe::t_node> ret;
  int size = v.size() - has_remainder(v);
  int level = v[0].level_ + 1;

  std::cout << "\n=====fold=====\n";
  std::cout << "size: " << size << std::endl;
  std::cout << "has_remainder: " << has_remainder(v) << std::endl;
  std::cout << "\n--------------" << std::endl;
  for (int i = 0; i + 1 < size; i += 2) {
    // if (v[i].getIval() > v[i + 1].getIval())
    if (CountedInt(v[i].getIval()) > CountedInt(v[i + 1].getIval()))
      ret.push_back(t_node(level, 0, v[i], v[i + 1]));
    else
      ret.push_back(t_node(level, 0, v[i + 1], v[i]));
  }
  if (size % 2) {
    if (has_remainder(v))
      ret.push_back(t_node(level, 0, v[size - 1], v[size]));
    else
      ret.push_back(t_node(level, 0, &v[size - 1], NULL));
  } else if (has_remainder(v))
    ret.push_back(t_node(level, 0, NULL, &v[size]));
  std::cout << ret << std::endl;
  std::cout << "\n--------------" << std::endl;
  return ret;
}

std::vector<PmergeMe::t_node>
PmergeMe::expand(const std::vector<PmergeMe::t_node> v) {
  std::vector<PmergeMe::t_node> ret;

  std::cout << "\n=====expand=====\n";
  int n = 2;
  ret.push_back(*v.begin()->low_);
  std::cout << "push begin.low: \n" << ret << "\n--------------" << std::endl;
  std::vector<PmergeMe::t_node>::const_iterator processed = v.begin();
  for (std::vector<PmergeMe::t_node>::const_iterator it = v.begin();
       it + has_remainder(v) != v.end(); it++) {
    if (it->high_)
      ret.push_back(*it->high_);
    std::cout << "push *it->high: \n" << ret << "\n--------------" << std::endl;
    if (has_remainder(v) && it == v.end() - 2 && v.back().high_) {
      std::cout << "insert remainder: \n";
      int index =
          PmergeMe::getIndexToInsert(ret.begin(), ret.end(), *v.back().high_);
      ret.insert(ret.begin() + index, *v.back().high_);
      std::cout << ret << "\n--------------" << std::endl;
    }
    if (ret.size() >= pow(2, n)) {
      for (std::vector<PmergeMe::t_node>::const_iterator it2 = it;
           it2 != processed && it2->low_; it2--) {
        std::cout << "insert *it2->low(high: " << it2->high_->getIval()
                  << "): \n";
        int index = PmergeMe::getIndexToInsert(
            ret.begin(), ret.begin() + pow(2, n) - 1, *it2->low_);
        // PmergeMe::getIndexToInsert(ret.begin(), ret.end() - 1, *it2->low_);
        ret.insert(ret.begin() + index, *it2->low_);
        std::cout << ret << "\n--------------" << std::endl;
      }
      n++;
      processed = it;
    }
  }
  if (has_remainder(v) && v.back().low_) {
    std::cout << "update remainder: \n";
    ret.push_back(*v.back().low_);
    std::cout << ret << "\n--------------" << std::endl;
  }
  return ret;
}

int PmergeMe::getIndexToInsert(
    std::vector<PmergeMe::t_node>::const_iterator begin,
    std::vector<PmergeMe::t_node>::const_iterator end, PmergeMe::t_node node) {
  int left = -1;
  int right = end - begin; // check
  std::cout << "right: " << right << std::endl;
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
    std::cout << "mid: " << mid << std::endl;
    if (CountedInt((begin + mid)->getIval()) > CountedInt(node.getIval()))
      right = mid;
    else
      left = mid;
  }
  return right;
}

//   printer
std::ostream& operator<<(std::ostream& os, const PmergeMe& pmm) {
  const std::vector<PmergeMe::t_node> data = pmm.getData();
  os << data;
  return os;
}

// s_node
//   constructor
PmergeMe::s_node::s_node() : level_(0), i_val_(0), high_(NULL), low_(NULL) {}

PmergeMe::s_node::s_node(int i_val)
    : level_(0), i_val_(i_val), high_(NULL), low_(NULL) {}

PmergeMe::s_node::s_node(int lvl, int ival, s_node* high_p, s_node* low_p)
    : level_(lvl), i_val_(ival) {
  high_ = high_p ? new t_node(*high_p) : NULL;
  low_ = low_p ? (new t_node(*low_p)) : NULL;
}

PmergeMe::s_node::s_node(int lvl, int ival, const s_node& a, const s_node& b)
    : level_(lvl), i_val_(ival), high_(new t_node(a)), low_(new t_node(b)) {}

PmergeMe::s_node::s_node(const s_node& other) {
  level_ = other.level_;
  i_val_ = other.i_val_;
  if (other.high_)
    high_ = new s_node(*other.high_);
  else
    high_ = NULL;
  if (other.low_)
    low_ = new s_node(*other.low_);
  else
    low_ = NULL;
}

//   destructor
PmergeMe::s_node::~s_node() {
  delete this->high_;
  delete this->low_;
}

//   operator
PmergeMe::s_node& PmergeMe::s_node::operator=(const PmergeMe::s_node& other) {
  if (this == &other)
    return *this;
  level_ = other.level_;
  i_val_ = other.i_val_;
  delete high_;
  delete low_;
  if (other.high_) {
    high_ = new s_node(*other.high_);
  } else
    high_ = NULL;
  if (other.low_)
    low_ = new s_node(*other.low_);
  else
    low_ = NULL;
  return *this;
}

//   accesser
int PmergeMe::s_node::getIval() const {
  int lvl = this->level_;
  const t_node* node = this;

  while (lvl--) {
    if (!node->high_)
      return node->i_val_;
    node = node->high_;
  }
  return node->i_val_;
}

//   printer
std::ostream& operator<<(std::ostream& os,
                         const std::vector<PmergeMe::t_node>& v) {
  for (std::vector<PmergeMe::t_node>::const_iterator it = v.begin();
       it != v.end(); it++)
    os << *it;
  return os;
}

std::ostream& operator<<(std::ostream& os, const PmergeMe::t_node& node) {
  if (node.level_ == 0) {
    os << node.i_val_ << " ";
    return os;
  }
  os << "\nL" << node.level_;
  os << "{ ";
  if (node.high_)
    os << *node.high_;
  else
    os << "NULL";
  os << " ";
  if (node.low_)
    os << *node.low_;
  else
    os << "NULL";
  os << "}\n";
  return os;
}
