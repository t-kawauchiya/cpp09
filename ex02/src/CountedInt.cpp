/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CountedInt.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:16:33 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/07 00:37:23 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CountedInt.hpp"
#include <iostream>

int CountedInt::cnt_ = 0;

CountedInt::CountedInt(int v) : v_(v) {}
CountedInt::CountedInt(const CountedInt& other) {
  v_ = other.v_;
}

CountedInt::~CountedInt() {}

CountedInt& CountedInt::operator=(const CountedInt& other) {
  if (this == &other)
    return *this;
  v_ = other.v_;
  return *this;
}

int CountedInt::getValue() const {
  return v_;
}

int CountedInt::getCnt() {
  return cnt_;
}

bool CountedInt::operator>(const CountedInt& rhs) const {
  ++cnt_;
  std::cout << "[comp] cnt: " << cnt_ << ", " << v_ << " > " << rhs.v_
            << std::endl;
  return v_ > rhs.v_;
}
