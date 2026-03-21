/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/21 20:34:20 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PMERGE_ME_H_
#define _PMERGE_ME_H_

#include <deque>
#include <ostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v);

struct Node {
  int level;
  int val;
  int highIdx;
  int lowIdx;

  Node();
  Node(int val);
  Node(int lvl, int val, int highIdx, int lowIdx);

  bool hasHigh() const;
  bool hasLow() const;
};

std::ostream& operator<<(std::ostream& os, const Node& node);
std::ostream& operator<<(std::ostream& os, const std::vector<Node>& v);
std::ostream& operator<<(std::ostream& os, const std::deque<Node>& v);

template <class OS, class CONTAINER>
void putNode(OS& os, const CONTAINER& pool, const Node& node, int indent) {
  if (node.level == 0) {
    os << node.val << " ";
    return;
  }
  os << std::string(indent, ' ');
  os << "L" << node.level << "{ " << (node.level > 1 ? "\n" : "");
  if (node.hasHigh())
    putNode(os, pool, pool[node.highIdx], indent + 2);
  else
    os << std::string(indent + 2, ' ') << "NULL"
       << (node.level > 1 ? "\n" : "");
  if (node.hasLow())
    putNode(os, pool, pool[node.lowIdx], indent + 2);
  else
    os << std::string(indent + 2, ' ') << "NULL"
       << (node.level > 1 ? "\n" : "");
  if (node.level > 1)
    os << std::string(indent, ' ');
  os << "}\n";
}

template <class OS>
void putResult(OS& os, const std::vector<Node>& pool,
               const std::vector<int>& result) {
  for (std::vector<int>::const_iterator it = result.begin(); it != result.end();
       it++)
    putNode(os, pool, pool[*it], 0);
  os << std::endl;
}

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
  PmergeMe(std::vector<int> input);

  ~PmergeMe();

  const std::vector<int>& getInput() const;

  bool compare(int a, int b);

  // for vector
  const std::vector<int>& getResult_v() const;
  void solve_v();
  bool hasRemainder(const std::vector<int>& v);
  int getIndexToInsert(std::vector<int>::const_iterator begin,
                       std::vector<int>::const_iterator end, int idx);
  std::vector<int>::iterator binaryInsert(std::vector<int>& v,
                                          std::vector<int>::iterator first,
                                          std::vector<int>::iterator end,
                                          int idx);

  // for deque
  const std::vector<int>& getResult_d() const;
  void solve_d();
  bool hasRemainder(const std::deque<int>& v);
  int getIndexToInsert(std::deque<int>::const_iterator begin,
                       std::deque<int>::const_iterator end, int idx);
  std::deque<int>::iterator binaryInsert(std::deque<int>& v,
                                         std::deque<int>::iterator first,
                                         std::deque<int>::iterator end,
                                         int idx);

private:
  PmergeMe();
  PmergeMe(const PmergeMe& other);
  PmergeMe& operator=(const PmergeMe& other);

  std::vector<int> input_;
  // for vector
  std::vector<int> solve_v(std::vector<int>& v);
  std::vector<int> fold_v(std::vector<int>& v);
  std::vector<int> expand_v(std::vector<int>& v);
  void processLowElems(std::vector<int>& ret,
                       std::vector<int>::const_iterator begin,
                       std::vector<int>::const_iterator end, size_t n);
  std::vector<Node> pool_v_;
  std::vector<int> result_v_;
  // for deque
  std::deque<int> solve_d(std::deque<int>& v);
  std::deque<int> fold_d(std::deque<int>& v);
  std::deque<int> expand_d(std::deque<int>& v);
  void processLowElems(std::deque<int>& ret,
                       std::deque<int>::const_iterator begin,
                       std::deque<int>::const_iterator end, size_t n);
  std::vector<Node> pool_d_;
  std::vector<int> result_d_;
};

std::ostream& operator<<(std::ostream& os, const PmergeMe& be);

// =============== for logging ===============
struct NullStream {
  template <class T> NullStream& operator<<(const T&) {
    return *this;
  }
  NullStream& operator<<(std::ostream& (*)(std::ostream&)) {
    return *this;
  }
};

static NullStream null_stream;

#ifdef DEBUG
#define DOUT std::cerr
#else
#define DOUT null_stream
#endif

#ifdef TRACE
#define TOUT std::cerr
#else
#define TOUT null_stream
#endif

const std::string separator = "\n--------------";

#endif
