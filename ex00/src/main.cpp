/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/16 20:44:20 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

void parseInput(const std::string& line, std::pair<time_t, double>& data);

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
    if (!std::getline(ifs, line))
      throw std::runtime_error("could not read file.");

    while (std::getline(ifs, line)) {
      try {
        // std::cout << "line: " << line << std::endl;
        std::pair<time_t, double> input;
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

void parseInput(const std::string& line, std::pair<time_t, double>& data) {
  std::istringstream iss(line);

  std::string date;
  if (!std::getline(iss, date, '|'))
    throw std::runtime_error("could not read file.");

  std::string value;
  if (!std::getline(iss, value))
    throw std::runtime_error("could not read file.");

  if (parseStringTime(date, data.first))
    throw std::runtime_error("bad input => " + date);

  std::stringstream ss(value);
  ss >> data.second;
  ss >> std::ws;
  if (!ss.eof())
    throw std::runtime_error("could not read file.3");

  if (data.second < 0)
    throw std::runtime_error("not a positive number.");
  if (data.second > static_cast<double>(std::numeric_limits<int>::max()))
    throw std::runtime_error("too large number.");
}
