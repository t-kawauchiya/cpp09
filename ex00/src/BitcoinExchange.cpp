/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 19:51:51 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/16 21:59:07 by takawauc         ###   ########.fr       */
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
  if (data_.empty())
    throw std::runtime_error("no price data");

  std::list<std::pair<time_t, double> >::const_iterator it = data_.begin();
  if (date < it->first)
    throw std::runtime_error("date is earlier than first available entry");

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

  std::string date;
  if (!std::getline(iss, date, ','))
    return EXIT_FAILURE;

  std::string value;
  if (!std::getline(iss, value, ','))
    return EXIT_FAILURE;

  if (parseStringTime(date, data.first))
    return EXIT_FAILURE;

  std::stringstream ss(value);
  ss >> data.second;
  ss >> std::ws;
  if (!ss.eof())
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int parseStringTime(std::string str, time_t& t) {
  int y, m, d;

  if (sscanf(str.c_str(), "%d-%d-%d", &y, &m, &d) != 3)
    return EXIT_FAILURE;
  std::tm tm = {};
  tm.tm_year = y - 1900;
  tm.tm_mon = m - 1;
  tm.tm_mday = d;
  std::tm orig = tm;
  t = std::mktime(&tm);
  if (t == (time_t)-1)
    return EXIT_FAILURE;
  if (tm.tm_year != orig.tm_year || tm.tm_mon != orig.tm_mon ||
      tm.tm_mday != orig.tm_mday)
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

  const std::size_t kDateFormatSize = sizeof("YYYY-MM-DD");
  char buf[kDateFormatSize];
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
