/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/25 20:54:40 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

std::pair<time_t, float> parseInput(std::string line);

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

  std::ifstream ifs(argv[1]);
  if (!ifs)
    std::cerr << "Error: could not open file." << std::endl;

  std::string line;
  std::pair<time_t, float> input;
  while (std::getline(ifs, line)) {
    try {
      input = parseInput(line);
    } catch (std::runtime_error e) {
      std::cerr << "Error: " << e.what() << "\n";
      continue;
    }
    std::cout << input.first << " => " << input.second << " = ";
    std::cout << btc.getPrice(input.first) * input.second << "\n";
  }
}

std::pair<time_t, float> parseInput(std::string line) {}
