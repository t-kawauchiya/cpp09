/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/07 18:13:52 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PMERGE_ME_H_
#define _PMERGE_ME_H_

#include <ctime>
#include <deque>
#include <iostream>
#include <ostream>
#include <vector>

const std::string separator = "\n--------------";

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v);

struct NullStream {
  template <class T> NullStream& operator<<(const T&) {
    return *this;
  }
  NullStream& operator<<(std::ostream& (*)(std::ostream&)) {
    return *this;
  }
};

#ifdef DEBUG
#define DOUT std::cerr
#else
#define DOUT null_stream
static NullStream null_stream;
#endif

class Node {
public:
  Node(const Node& other);
  Node(int i_val);
  Node(int lvl, int i_val, const Node& high, const Node& low);
  Node(int lvl, int i_val, Node* high_p, Node* low_p);

  ~Node();

  Node& operator=(const Node& other);

  int getLevel() const;
  int getVal() const;
  int getTopVal() const;
  Node* getHigh() const;
  Node* getLow() const;
  void setHigh(Node*);
  void setLow(Node*);

private:
  Node();
  int level_;
  int val_;
  Node* high_;
  Node* low_;
};

std::ostream& operator<<(std::ostream& os, const Node& node);
std::ostream& operator<<(std::ostream& os, const std::vector<Node>& v);
std::ostream& operator<<(std::ostream& os, const std::deque<Node>& v);

class CountedInt {
public:
  CountedInt(int v);
  CountedInt(const CountedInt& other);
  ~CountedInt();

  int getValue() const;
  static int getCnt();
  static void resetCnt();

  bool operator>(const CountedInt& rhs) const;

private:
  CountedInt();
  CountedInt& operator=(const CountedInt& other);
  int v_;
  static int cnt_;
};

class PmergeMeVector {
public:
  PmergeMeVector();
  PmergeMeVector(std::vector<Node> data);
  PmergeMeVector(std::vector<int> data);
  PmergeMeVector(const PmergeMeVector& other);

  ~PmergeMeVector();

  std::vector<Node> getData() const;
  std::vector<int> getResult() const;

  void solve();

  PmergeMeVector& operator=(const PmergeMeVector& other);

  static bool has_remainder(std::vector<Node> v);
  static int getIndexToInsert(std::vector<Node>::const_iterator begin,
                              std::vector<Node>::const_iterator end, Node node);
  static std::vector<Node>::iterator
  binaryInsert(std::vector<Node>& v, std::vector<Node>::iterator first,
               std::vector<Node>::iterator end, const Node& item);

private:
  std::vector<Node> solve(const std::vector<Node>& v);
  static std::vector<Node> fold(const std::vector<Node> v);
  static std::vector<Node> expand(const std::vector<Node> v);
  std::vector<Node> data_;
};

std::ostream& operator<<(std::ostream& os, const PmergeMeVector& be);

class PmergeMeDeque {
public:
  PmergeMeDeque();
  PmergeMeDeque(std::deque<Node> data);
  PmergeMeDeque(std::vector<int> data);
  PmergeMeDeque(const PmergeMeDeque& other);

  ~PmergeMeDeque();

  std::deque<Node> getData() const;
  std::vector<int> getResult() const;
  void solve();

  PmergeMeDeque& operator=(const PmergeMeDeque& other);

  static bool has_remainder(std::deque<Node> v);
  static int getIndexToInsert(std::deque<Node>::const_iterator begin,
                              std::deque<Node>::const_iterator end, Node node);
  static std::deque<Node>::iterator
  binaryInsert(std::deque<Node>& v, std::deque<Node>::iterator first,
               std::deque<Node>::iterator end, const Node& item);

private:
  std::deque<Node> solve(const std::deque<Node>& v);
  static std::deque<Node> fold(const std::deque<Node> v);
  static std::deque<Node> expand(const std::deque<Node> v);
  std::deque<Node> data_;
};

std::ostream& operator<<(std::ostream& os, const PmergeMeDeque& be);

void swap(Node& a, Node& b);
bool compare(const Node& a, const Node& b);

#endif
