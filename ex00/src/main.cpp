/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/27 13:23:46 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

void parseInput(std::string filename, const BitcoinExchange &btc);
void parseInputLine(std::string line, time_t &t, double &f);
std::string parseTmToString(time_t t);

int main(int argc, char **argv) {
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
    parseInput(argv[1], btc);
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void parseInput(std::string filename, const BitcoinExchange &btc) {
  std::ifstream ifs(filename.c_str());
  if (!ifs)
    throw std::runtime_error("could not open file.");

  std::string line;
  std::pair<time_t, double> input;

  if (!std::getline(ifs, line))
    throw std::runtime_error("could not read file.");

  while (std::getline(ifs, line)) {
    try {
      // std::cout << "line: " << line << std::endl;
      parseInputLine(line, input.first, input.second);
      std::string date = parseTmToString(input.first);
      std::cout << date << " => " << input.second << " = ";
      std::cout << btc.getPrice(input.first) * input.second << "\n";
    } catch (std::runtime_error e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  }
}

void parseInputLine(std::string line, time_t &t, double &f) {

  std::istringstream iss(line);
  if (!iss)
    throw std::runtime_error("could not read file.1");

  std::string csv_item;
  if (!std::getline(iss, csv_item, '|'))
    throw std::runtime_error("could not read file.2");
  if (parseStringTime(csv_item, t))
    throw std::runtime_error("bad input => " + csv_item);

  if (!std::getline(iss, csv_item, '|'))
    throw std::runtime_error("could not read file.3");
  std::stringstream ss(csv_item);
  ss >> f;
  if (f < 0)
    throw std::runtime_error("not a positive number.");

  if (f > (double)std::numeric_limits<int>::max())
    throw std::runtime_error("too large a number.");

  if (ss.fail())
    throw std::runtime_error("could not read file.4");
}

std::string parseTmToString(time_t t) {
  std::tm *tm = std::localtime(&t);
  if (!tm)
    return std::string();

  char buf[32];
  if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm) == 0)
    return std::string();
  return std::string(buf);
}

int parseStringTime(std::string str, time_t &t) {
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
