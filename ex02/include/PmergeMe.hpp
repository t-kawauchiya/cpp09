/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/27 23:35:25 by takawauc         ###   ########.fr       */
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
    int level;
    int i_val;
    std::pair<s_node*, s_node*> p_val;

    s_node();
    s_node(int lvl, int i_val, s_node* a, s_node* b);
    // ~s_node();
    int getIval() const;
    s_node& operator=(const s_node& other);
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
  void solve(std::vector<t_node>& v);

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

#endif
