/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/27 23:38:21 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <cctype>
#include <iostream>
#include <vector>

PmergeMe::PmergeMe() : data_() {}

PmergeMe::PmergeMe(std::vector<t_node> data) : data_(data) {}

PmergeMe::PmergeMe(std::vector<int> input) {
  std::vector<int>::const_iterator it = input.begin();
  while (it != input.end()) {
    t_node node(0, *it, (s_node*)0, (s_node*)0);
    data_.push_back(node);
    it++;
  }
}

PmergeMe::PmergeMe(const PmergeMe& other) {
  data_ = other.data_;
}

PmergeMe::~PmergeMe() {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
  if (this == &other)
    return *this;
  data_ = other.data_;
  return *this;
}

std::vector<PmergeMe::t_node> PmergeMe::getData() const {
  return data_;
}

std::vector<int> PmergeMe::getResult() const {
  std::vector<int> res;
  std::vector<PmergeMe::t_node>::const_iterator it = data_.begin();
  while (it != data_.end()) {
    res.push_back(it->i_val);
    it++;
  }
  return res;
}

void PmergeMe::solve() {
  solve(this->data_);
}

void PmergeMe::swap(PmergeMe::t_node& a, PmergeMe::t_node& b) {
  PmergeMe::t_node c(a);
  a = b;
  b = c;
}

void PmergeMe::solve(std::vector<PmergeMe::t_node>& v) {
  if (v.size() == 2) {
    if (v[0].getIval() > v[1].getIval())
      PmergeMe::swap(v[0], v[1]);
    return;
  }
  std::vector<PmergeMe::t_node> folded = fold(v);
  std::cout << "\n=====folded=====\n" << folded;
  // solve(folded);
  // v = expand(folded);
}

std::vector<PmergeMe::t_node> PmergeMe::fold(std::vector<PmergeMe::t_node> v) {

  std::vector<PmergeMe::t_node> ret;
  t_node node;
  int n = v.size();
  std::cout << "v: " << v;

  for (int i = 0; i + 1 < n; i += 2) {
    if (v[i].getIval() > v[i + 1].getIval())
      node = t_node(v[i].level + 1, 0, &v[i], &v[i + 1]);
    else
      node = t_node(v[i].level + 1, 0, &v[i + 1], &v[i]);
    ret.push_back(node);
  }
  std::cout << "ret.size: " << ret.size() << std::endl;
  return ret;
}

std::vector<PmergeMe::t_node>
PmergeMe::expand(const std::vector<PmergeMe::t_node> v) {
  std::vector<PmergeMe::t_node> ret(v);
  if (v[0].level == 0)
    throw std::runtime_error("internal error.");

  std::vector<PmergeMe::t_node>::const_iterator it = v.begin();
  ret.insert(ret.begin(), *it->p_val.second);
  it++;
  while (it != v.end()) {
    ret.insert(ret.begin() + PmergeMe::getIndexToInsert(ret.begin(), ret.end(),
                                                        *it->p_val.second),
               *it->p_val.second);
  }
  return ret;
}

int PmergeMe::getIndexToInsert(
    std::vector<PmergeMe::t_node>::const_iterator begin,
    std::vector<PmergeMe::t_node>::const_iterator end, PmergeMe::t_node node) {
  int left = 0;
  int right = end - begin;
  int ret = 0;
  while (right >= left) {
    ret = left + (right - left) / 2;
    if ((begin + ret)->getIval() == node.getIval())
      return ret;
    else if ((begin + ret)->getIval() > node.getIval())
      right = ret - 1;
    else if ((begin + ret)->getIval() < node.getIval())
      left = ret + 1;
  }
  return ret;
}

std::ostream& operator<<(std::ostream& os, const PmergeMe& pmm) {
  std::vector<PmergeMe::t_node> data = pmm.getData();
  std::vector<PmergeMe::t_node>::const_iterator it = data.begin();

  while (it != data.end()) {
    os << *it << "\n";
    it++;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::vector<PmergeMe::t_node>& v) {
  for (std::vector<PmergeMe::t_node>::const_iterator it = v.begin();
       it != v.end(); it++)
    os << *it << "\n";
  return os;
}

PmergeMe::s_node::s_node() : level(0), i_val(0), p_val(NULL, NULL) {}

PmergeMe::s_node::s_node(int lvl, int ival, s_node* a, s_node* b)
    : level(lvl), i_val(ival), p_val(a, b) {}

int PmergeMe::s_node::getIval() const {
  int lvl = this->level;
  t_node node = *this;

  while (lvl--) {
    node = *node.p_val.first;
  }
  return node.i_val;
}

PmergeMe::s_node& PmergeMe::s_node::operator=(const PmergeMe::s_node& other) {
  if (this == &other)
    return *this;
  level = other.level;
  i_val = other.i_val;
  p_val = other.p_val;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const PmergeMe::t_node& node) {
  os << "level: " << node.level << ", ";
  if (node.level == 0)
    os << "i_val: " << node.i_val;
  else
    os << "p_val: { " << *((PmergeMe::t_node*)node.p_val.first) << ", ";
  // << *((PmergeMe::s_node*)node.p_val.second) << " }";

  return os;
}
