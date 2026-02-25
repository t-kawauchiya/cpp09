/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/25 20:55:26 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
  this->data_ = other.data_;
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
  if (this == &other)
    return *this;
  this->data_ = other.data_;
  return *this;
}

void BitcoinExchange::addData(std::pair<time_t, int> data) {
  data_.push_back(data);
}

int BitcoinExchange::getPrice(time_t date) {
  std::list<std::pair<time_t, int> >::iterator it = data_.begin();
  while (it != data_.end()) {
    if (it->first > date)
      return (--it)->second;
  }
  return (--it)->second;
}

int BitcoinExchange::init(std::string filepath) {}
