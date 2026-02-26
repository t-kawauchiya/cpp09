/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/26 19:26:04 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <utility>

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

std::list<std::pair<time_t, int> > BitcoinExchange::getData() const {
  return this->data_;
}

void BitcoinExchange::addData(std::pair<time_t, int> data) {
  data_.push_back(data);
}

int BitcoinExchange::getPrice(time_t date) const {
  std::list<std::pair<time_t, int> >::const_iterator it = data_.begin();
  while (it != data_.end()) {
    if (it->first > date)
      return (--it)->second;
    it++;
  }
  return (--it)->second;
}

int parseLine(std::string line, std::pair<time_t, int> &data);

int BitcoinExchange::init(std::string filepath) {
  std::ifstream ifs(filepath.c_str());
  if (!ifs)
    return EXIT_FAILURE;
  std::string line;
  if (!std::getline(ifs, line))
    return EXIT_FAILURE;
  while (std::getline(ifs, line)) {
    std::pair<time_t, int> data;
    if (parseLine(line, data))
      return EXIT_FAILURE;
    if (ifs.fail())
      return EXIT_FAILURE;
    data_.push_back(data);
  }
  return EXIT_SUCCESS;
}

int parseLine(std::string line, std::pair<time_t, int> &data) {
  std::istringstream iss(line);
  if (!iss)
    return EXIT_FAILURE;

  std::string csv_item;
  if (!std::getline(iss, csv_item, ','))
    return EXIT_FAILURE;
  if (parseStringTime(csv_item, data.first))
    return EXIT_FAILURE;

  if (!std::getline(iss, csv_item, ','))
    return EXIT_FAILURE;
  std::stringstream ss(csv_item);
  ss >> data.second;

  if (ss.fail())
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

std::ostream &operator<<(std::ostream &os, const BitcoinExchange &be) {
  std::list<std::pair<time_t, int> > data = be.getData();

  std::list<std::pair<time_t, int> >::iterator it = data.begin();
  while (it != data.end()) {
    os << "{ " << (*it).first << ", " << (*it).second << " }, ";
    it++;
  }
  return os;
}
