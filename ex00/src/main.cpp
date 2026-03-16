/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/16 19:12:23 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

void parseInput(std::string line, std::pair<time_t, double>& data);
void putData(BitcoinExchange btc, std::pair<time_t, double> data);
std::string parseTmToString(time_t t);

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cerr << "Error: could not open file." << std::endl;
    return EXIT_FAILURE;
  }
  if (argc != 2) {
    std::cerr << "Error: can not accept multiple files." << std::endl;
    return EXIT_FAILURE;
  }
  try {
    BitcoinExchange btc(DATA_FILE_PATH);
    std::ifstream ifs(argv[1]);
    if (!ifs)
      throw std::runtime_error("could not open file.");
    std::string line;
    std::pair<time_t, double> input;
    if (!std::getline(ifs, line))
      throw std::runtime_error("could not read file.");

    while (std::getline(ifs, line)) {
      try {
        std::cout << "line: " << line << std::endl;
        parseInput(line, input);
        btc.putData(input);
      } catch (std::runtime_error e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void parseInput(std::string line, std::pair<time_t, double>& data) {
  std::istringstream iss(line);
  if (!iss)
    throw std::runtime_error("could not read file.");

  std::string date;
  if (!std::getline(iss, date, '|'))
    throw std::runtime_error("could not read file.");
  if (parseStringTime(date, data.first))
    throw std::runtime_error("bad input => " + date);

  std::string value;
  if (!std::getline(iss, value, '|'))
    throw std::runtime_error("could not read file.");
  std::stringstream ss(value);
  ss >> data.second;
  if (data.second < 0)
    throw std::runtime_error("not a positive number.");

  if (data.second > (double)std::numeric_limits<int>::max())
    throw std::runtime_error("too large a number.");

  if (ss.fail())
    throw std::runtime_error("could not read file.4");
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
