/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/27 14:41:22 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RPN_H_
#define _RPN_H_

#include <ctime>
#include <ostream>
#include <stack>

class RPN {
public:
  RPN();
  RPN(std::stack<int> stack);
  RPN(const RPN &other);

  ~RPN();

  RPN &operator=(const RPN &other);

  std::stack<int> getStack() const;

  int getTop();

  void acceptToken(std::string Token);

private:
  void pushNum(int num);
  void add();
  void sub();
  void mul();
  void div();

  std::stack<int> stack_;
};

std::ostream &operator<<(std::ostream &os, const RPN &be);

#endif
