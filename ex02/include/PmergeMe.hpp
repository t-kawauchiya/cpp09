/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/07 01:08:03 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PMERGE_ME_H_
#define _PMERGE_ME_H_

#include <ctime>
#include <ostream>
#include <vector>

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

class PmergeMe {
public:
  PmergeMe();
  PmergeMe(std::vector<Node> data);
  PmergeMe(std::vector<int> data);
  PmergeMe(const PmergeMe& other);

  ~PmergeMe();

  std::vector<Node> getData() const;
  std::vector<int> getResult() const;

  void solve();

  PmergeMe& operator=(const PmergeMe& other);

  static bool has_remainder(std::vector<Node> v);

private:
  std::vector<Node> solve(const std::vector<Node>& v);

  static void swap(Node& a, Node& b);
  static bool compare(const Node& a, const Node& b);
  static std::vector<Node> fold(const std::vector<Node> v);
  static std::vector<Node> expand(const std::vector<Node> v);
  static int getIndexToInsert(std::vector<Node>::const_iterator begin,
                              std::vector<Node>::const_iterator end, Node node);
  std::vector<Node> data_;
};

std::ostream& operator<<(std::ostream& os, const PmergeMe& be);

#endif
