/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/06 22:00:55 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PMERGE_ME_H_
#define _PMERGE_ME_H_

#include <ctime>
#include <ostream>
#include <vector>

class PmergeMe {
public:
  typedef struct s_node {
    s_node();
    s_node(const s_node& other);
    s_node(int i_val);
    s_node(int lvl, int i_val, const s_node& high, const s_node& low);
    s_node(int lvl, int ival, s_node* high_p, s_node* low_p);

    ~s_node();

    s_node& operator=(const s_node& other);

    int getIval() const;

    int level_;
    int i_val_;
    s_node* high_;
    s_node* low_;
  } t_node;

  PmergeMe();
  PmergeMe(std::vector<t_node> data);
  PmergeMe(std::vector<int> data);
  PmergeMe(const PmergeMe& other);

  ~PmergeMe();

  std::vector<t_node> getData() const;

  std::vector<int> getResult() const;

  void solve();

  PmergeMe& operator=(const PmergeMe& other);

private:
  std::vector<t_node> solve(const std::vector<t_node>& v);

  static bool has_remainder(std::vector<PmergeMe::t_node> v);
  static void swap(PmergeMe::t_node& a, PmergeMe::t_node& b);
  static std::vector<t_node> fold(const std::vector<t_node> v);
  static std::vector<t_node> expand(const std::vector<t_node> v);
  static int getIndexToInsert(std::vector<t_node>::const_iterator begin,
                              std::vector<t_node>::const_iterator end,
                              t_node node);
  std::vector<t_node> data_;
};

std::ostream& operator<<(std::ostream& os, const PmergeMe& be);
std::ostream& operator<<(std::ostream& os, const PmergeMe::t_node& node);
std::ostream& operator<<(std::ostream& os,
                         const std::vector<PmergeMe::t_node>& v);

#endif
