/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/08 19:32:08 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PMERGE_ME_H_
#define _PMERGE_ME_H_

#include <ctime>
#include <deque>
#include <iostream>
#include <ostream>
#include <vector>

class Node {
public:
  Node(const Node& other);
  Node(int val);
  Node(int lvl, int val, Node* high_p, Node* low_p);

  ~Node();

  Node& operator=(const Node& other);

  int getLevel() const;
  int getVal() const;
  Node* getHigh() const;
  Node* getLow() const;
  void setHigh(Node*);
  void setLow(Node*);

  int getTopVal() const;

private:
  Node();
  int level_;
  int val_;
  Node* high_;
  Node* low_;
};

std::ostream& operator<<(std::ostream& os, const Node& node);

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

class PmergeMe {
public:
  PmergeMe();
  PmergeMe(std::vector<int> input);
  PmergeMe(const PmergeMe& other);

  ~PmergeMe();

  std::vector<int> getInput() const;
  std::vector<int> getResultVector() const;
  std::vector<int> getResultDeque() const;

  void solveVector();
  void solveDeque();

  PmergeMe& operator=(const PmergeMe& other);

  static bool hasRemainder(std::vector<Node> v);
  static int getIndexToInsert(std::vector<Node>::const_iterator begin,
                              std::vector<Node>::const_iterator end, Node node);
  static std::vector<Node>::iterator
  binaryInsert(std::vector<Node>& v, std::vector<Node>::iterator first,
               std::vector<Node>::iterator end, const Node& item);

  static bool hasRemainder(std::deque<Node> v);
  static int getIndexToInsert(std::deque<Node>::const_iterator begin,
                              std::deque<Node>::const_iterator end, Node node);
  static std::deque<Node>::iterator
  binaryInsert(std::deque<Node>& v, std::deque<Node>::iterator first,
               std::deque<Node>::iterator end, const Node& item);

private:
  std::vector<Node> solveVector(const std::vector<Node>& v);
  static std::vector<Node> foldVector(const std::vector<Node> v);
  static std::vector<Node> expandVector(const std::vector<Node> v);

  std::deque<Node> solveDeque(const std::deque<Node>& v);
  static std::deque<Node> foldDeque(const std::deque<Node> v);
  static std::deque<Node> expandDeque(const std::deque<Node> v);
  std::vector<int> input_;
  std::vector<int> result_vector_;
  std::vector<int> result_deque_;
};

std::ostream& operator<<(std::ostream& os, const PmergeMe& be);

void swap(Node& a, Node& b);
bool compare(const Node& a, const Node& b);

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v);
std::ostream& operator<<(std::ostream& os, const std::vector<Node>& v);
std::ostream& operator<<(std::ostream& os, const std::deque<Node>& v);

// =============== for logging ===============
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

const std::string separator = "\n--------------";

#endif
