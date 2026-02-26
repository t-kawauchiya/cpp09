/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/26 19:25:01 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

int parseInput(std::string filename, const BitcoinExchange &btc);
int parseInputLine(std::string line, time_t &t, float &f);
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

  BitcoinExchange btc;

  if (btc.init(DATA_FILE_PATH))
    return EXIT_FAILURE;
  return parseInput(argv[1], btc);
}

int parseInput(std::string filename, const BitcoinExchange &btc) {
  std::ifstream ifs(filename.c_str());
  if (!ifs)
    std::cerr << "Error: could not open file." << std::endl;

  std::string line;
  std::pair<time_t, float> input;

  if (!std::getline(ifs, line))
    return EXIT_FAILURE;

  while (std::getline(ifs, line)) {
    if (parseInputLine(line, input.first, input.second))
      continue;
    std::string date = parseTmToString(input.first);
    if (date.empty())
      continue;
    std::cout << input.first << " => " << input.second << " = ";
    std::cout << btc.getPrice(input.first) * input.second << "\n";
  }
  return EXIT_SUCCESS;
}

int parseInputLine(std::string line, time_t &t, float &f) {

  std::istringstream iss(line);
  if (!iss)
    return EXIT_FAILURE;

  std::cout << line << std::endl;
  std::string csv_item;
  if (!std::getline(iss, csv_item, '|'))
    return EXIT_FAILURE;
  if (parseStringTime(csv_item, t))
    return EXIT_FAILURE;

  if (!std::getline(iss, csv_item, '|'))
    return EXIT_FAILURE;
  std::stringstream ss(csv_item);
  ss >> f;

  if (ss.fail())
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
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
  t = std::mktime(&tm);
  if (t < 0)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
