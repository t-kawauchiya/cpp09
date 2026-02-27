/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/27 16:43:03 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stack>

RPN::RPN() : stack_(std::stack<int>()) {}

RPN::RPN(std::stack<int> stack) : stack_(stack) {}

RPN::RPN(const RPN& other) {
  this->stack_ = other.stack_;
}

RPN::~RPN() {}

RPN& RPN::operator=(const RPN& other) {
  if (this == &other)
    return *this;
  this->stack_ = other.stack_;
  return *this;
}

std::stack<int> RPN::getStack() const {
  return stack_;
}

int RPN::getTop() {
  if (stack_.empty())
    throw std::runtime_error("accessing top of empty stack.");
  return stack_.top();
}

bool isAccebtableChar(char c) {
  if (std::isdigit(c))
    return true;
  if (c == '+' || c == '-' || c == '*' || c == '/')
    return true;
  return false;
}

void RPN::acceptToken(std::string token) {
  if (token.size() != 1 || !isAccebtableChar(token.c_str()[0]))
    throw std::runtime_error("bad input : \"" + token + "\"");
  char c = token.c_str()[0];
  if (std::isdigit(c)) {
    pushNum(c - '0');
    return;
  }
  switch (c) {
  case '+':
    add();
    break;
  case '-':
    sub();
    break;
  case '*':;
    mul();
    break;
  case '/':;
    div();
    break;
  default:
    throw std::runtime_error("bad input : " + token);
  }
}

void RPN::pushNum(int num) {
  stack_.push(num);
}

void RPN::add() {
  int a, b;

  a = stack_.top();
  stack_.pop();
  b = stack_.top();
  stack_.pop();
  stack_.push(b + a);
}

void RPN::sub() {
  int a, b;

  a = stack_.top();
  stack_.pop();
  b = stack_.top();
  stack_.pop();
  stack_.push(b - a);
}

void RPN::mul() {
  int a, b;

  a = stack_.top();
  stack_.pop();
  b = stack_.top();
  stack_.pop();
  stack_.push(b * a);
}

void RPN::div() {
  int a, b;

  a = stack_.top();
  stack_.pop();
  b = stack_.top();
  stack_.pop();
  stack_.push(b / a);
}

std::ostream& operator<<(std::ostream& os, const RPN& rpn) {

  std::stack<int> st = rpn.getStack();
  while (!st.empty()) {
    std::cout << st.top() << " ";
    st.pop();
  }
  return os;
}
