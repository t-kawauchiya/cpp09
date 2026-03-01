/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/01 09:25:10 by takawauc         ###   ########.fr       */
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
    res.push_back(it->i_val);
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
  std::vector<PmergeMe::t_node> ret(v);
  if (v.size() == 2) {
    if (ret[0].getIval() > ret[1].getIval())
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

std::vector<PmergeMe::t_node> PmergeMe::fold(std::vector<PmergeMe::t_node> v) {

  std::vector<PmergeMe::t_node> ret;
  int n = v.size();

  std::cout << "\n=====fold=====\n";
  for (int i = 0; i + 1 < n; i += 2) {
    // if (v[i].getIval() > v[i + 1].getIval())
    if (CountedInt(v[i].getIval()) > CountedInt(v[i + 1].getIval()))
      ret.push_back(t_node(v[i].level + 1, 0, v[i], v[i + 1]));
    else
      ret.push_back(t_node(v[i].level + 1, 0, v[i + 1], v[i]));
  }
  if (n % 2)
    ret.push_back(t_node(v[n - 1]));
  std::cout << "ret: \n" << ret << std::endl;
  return ret;
}

std::vector<PmergeMe::t_node>
PmergeMe::expand(const std::vector<PmergeMe::t_node> v) {
  std::vector<PmergeMe::t_node> ret;

  std::cout << "\n=====expand=====\n";
  int n = 2;
  if (v[0].level != 0) {
    ret.push_back(*v.begin()->low);
  }
  std::vector<PmergeMe::t_node>::const_iterator processed = v.begin();
  for (std::vector<PmergeMe::t_node>::const_iterator it = v.begin();
       it != v.end(); it++) {
    if (it->level == 0)
      ret.push_back(*it);
    else
      ret.push_back(*it->high);
    if (ret.size() >= pow(2, n) || it != v.end()) {
      for (std::vector<PmergeMe::t_node>::const_iterator it2 = it;
           it2 != processed && it->low; it2--) {
        int index =
            PmergeMe::getIndexToInsert(ret.begin(), ret.end(), *it->low);
        ret.insert(ret.begin() + index, *it->low);
      }
      n++;
      processed = it;
    }
  }
  std::cout << "ret: " << ret << std::endl;
  return ret;
}

// std::vector<PmergeMe::t_node>
// PmergeMe::expand(const std::vector<PmergeMe::t_node> v) {
//   std::vector<PmergeMe::t_node> ret;
//   std::cout << "\n=====expand=====" << std::endl;
//   std::cout << "input: " << v << std::endl;
//   if (v[0].level != 0) {
//     ret.push_back(*v.begin()->low);
//     std::cout << "push begin.low: " << ret << std::endl;
//   }
//   for (std::vector<PmergeMe::t_node>::const_iterator it = v.begin();
//        it != v.end(); it++) {
//     if (it->level == 0)
//       ret.push_back(*it);
//     else
//       ret.push_back(*it->high);
//     std::cout << "push *it->high: " << ret << std::endl;
//   }
//   for (std::vector<PmergeMe::t_node>::const_iterator it = v.begin() + 1;
//        it != v.end() && it->low; it++) {
//     int index = PmergeMe::getIndexToInsert(ret.begin(), ret.end(), *it->low);
//     ret.insert(ret.begin() + index, *it->low);
//     std::cout << "push *it->low : " << ret << std::endl;
//   }
//   return ret;
// }

int PmergeMe::getIndexToInsert(
    std::vector<PmergeMe::t_node>::const_iterator begin,
    std::vector<PmergeMe::t_node>::const_iterator end, PmergeMe::t_node node) {
  int left = -1;
  int right = end - begin + 1;
  while (right - left > 1) {
    int mid = left + (right - left) / 2;
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
PmergeMe::s_node::s_node() : level(0), i_val(0), high(NULL), low(NULL) {}

PmergeMe::s_node::s_node(int i_val)
    : level(0), i_val(i_val), high(NULL), low(NULL) {}

PmergeMe::s_node::s_node(int lvl, int ival, const s_node& a, const s_node& b)
    : level(lvl), i_val(ival), high(new t_node(a)), low(new t_node(b)) {}

PmergeMe::s_node::s_node(const s_node& other) {
  level = other.level;
  i_val = other.i_val;
  if (other.high)
    high = new s_node(*other.high);
  else
    high = NULL;
  if (other.low)
    low = new s_node(*other.low);
  else
    low = NULL;
}

//   destructor
PmergeMe::s_node::~s_node() {
  delete this->high;
  delete this->low;
}

//   operator
PmergeMe::s_node& PmergeMe::s_node::operator=(const PmergeMe::s_node& other) {
  if (this == &other)
    return *this;
  level = other.level;
  i_val = other.i_val;
  delete high;
  delete low;
  if (other.high) {
    high = new s_node(*other.high);
  } else
    high = NULL;
  if (other.low)
    low = new s_node(*other.low);
  else
    low = NULL;
  return *this;
}

//   accesser
int PmergeMe::s_node::getIval() const {
  int lvl = this->level;
  const t_node* node = this;

  while (lvl--) {
    node = node->high;
  }
  return node->i_val;
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
  if (node.level == 0)
    os << node.i_val << " ";
  else {
    os << "\nL" << node.level;
    os << "{ " << *node.high << " " << *node.low << "}\n";
  }
  return os;
}
