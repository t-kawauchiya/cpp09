/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/18 18:03:02 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

void parseInput(const std::string& line, std::pair<time_t, double>& data);

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./btc <filepath>" << std::endl;
    return EXIT_FAILURE;
  }
  try {
    BitcoinExchange btc(DATA_FILE_PATH);
    std::ifstream ifs(argv[1]);
    if (!ifs)
      throw std::runtime_error("failed to open input file.");

    std::string line;
    const std::string header("date | value");

    while (std::getline(ifs, line)) {
      try {
        // std::cout << "line: " << line << std::endl;
        if (line.empty() || line.compare(0, header.length(), header) == 0)
          continue;
        std::pair<time_t, double> input;
        parseInput(line, input);
        btc.putData(input);
      } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void parseInput(const std::string& line, std::pair<time_t, double>& data) {
  std::istringstream iss(line);

  std::string date;
  if (!std::getline(iss, date, '|'))
    throw std::runtime_error("failed to read file.");

  if (parseStringTime(date, data.first))
    throw std::invalid_argument("bad input => " + date);

  std::string value;
  if (!std::getline(iss, value))
    throw std::invalid_argument("bad input => " + line);

  std::stringstream ss(value);
  ss >> data.second;
  ss >> std::ws;
  if (!ss.eof())
    throw std::invalid_argument("bad input => " + line);

  if (data.second < 0)
    throw std::invalid_argument("not a positive number.");
  if (data.second > static_cast<double>(1000))
    throw std::invalid_argument("too large number.");
}
