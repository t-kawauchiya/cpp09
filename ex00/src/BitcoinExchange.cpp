/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/16 19:39:38 by takawauc         ###   ########.fr       */
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

BitcoinExchange::BitcoinExchange(std::string filename) {
  this->importData(filename);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
  this->data_ = other.data_;
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
  if (this == &other)
    return *this;
  this->data_ = other.data_;
  return *this;
}

std::list<std::pair<time_t, double> > BitcoinExchange::getData() const {
  return this->data_;
}

void BitcoinExchange::addData(std::pair<time_t, double> data) {
  data_.push_back(data);
}

double BitcoinExchange::getPrice(time_t date) const {
  std::list<std::pair<time_t, double> >::const_iterator it = data_.begin();
  while (it != data_.end()) {
    if (it->first > date)
      return (--it)->second;
    it++;
  }
  return (--it)->second;
}

static int importLine(std::string line, std::pair<time_t, double>& data);

int BitcoinExchange::importData(std::string filepath) {
  std::ifstream ifs(filepath.c_str());
  if (!ifs)
    return EXIT_FAILURE;

  std::string line;
  if (!std::getline(ifs, line))
    return EXIT_FAILURE;
  while (std::getline(ifs, line)) {
    std::pair<time_t, double> data;
    if (importLine(line, data))
      return EXIT_FAILURE;
    data_.push_back(data);
  }
  return EXIT_SUCCESS;
}

static int importLine(std::string line, std::pair<time_t, double>& data) {
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

static std::string parseTmToString(time_t t);

void BitcoinExchange::putData(std::pair<time_t, double> data) {
  std::string date = parseTmToString(data.first);
  std::cout << date << " => " << data.second << " = ";
  std::cout << getPrice(data.first) * data.second << "\n";
}

static std::string parseTmToString(time_t t) {
  std::tm* tm = std::localtime(&t);
  if (!tm)
    return std::string();

  char buf[32];
  if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm) == 0)
    return std::string();
  return std::string(buf);
}

std::ostream& operator<<(std::ostream& os, const BitcoinExchange& be) {
  std::list<std::pair<time_t, double> > data = be.getData();

  std::list<std::pair<time_t, double> >::iterator it = data.begin();
  while (it != data.end()) {
    os << "{ " << (*it).first << ", " << (*it).second << " }\n";
    it++;
  }
  return os;
}
