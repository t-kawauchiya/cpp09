/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/19 12:09:18 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <time.h>

std::vector<int> parseStrArrToVector(char** sarr, int size);

double getDuration(clock_t start, clock_t end);

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cerr << "Error: PmergeMe needs args." << std::endl;
    return EXIT_FAILURE;
  }
  try {
    std::vector<int> input = parseStrArrToVector(argv + 1, argc - 1);
    PmergeMe pmm(input);

    clock_t start_v = clock();
    pmm.solveVector();
    clock_t end_v = clock();

    clock_t start_d = clock();
    pmm.solveDeque();
    clock_t end_d = clock();

    std::cout << "Before: " << input << std::endl;
    std::cout << "After:  " << pmm.getResultVector() << std::endl;
    DOUT << "After:  " << pmm.getResultDeque() << std::endl;
    std::cout << "Time to process a range of " << input.size()
              << " elements with std::vector : " << std::fixed
              << std::setprecision(5) << getDuration(start_v, end_v) << "us\n";
    std::cout << "Time to process a range of " << input.size()
              << " elements with std::deque  : " << std::fixed
              << std::setprecision(5) << getDuration(start_d, end_d) << "us\n";
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

double getDuration(clock_t start, clock_t end) {
  return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000 * 1000;
}

std::vector<int> parseStrArrToVector(char** sarr, int size) {
  std::vector<int> ret;
  for (int i = 0; i < size; i++) {
    std::stringstream ss(sarr[i]);
    int elem;
    ss >> elem;
    if (!ss)
      throw std::invalid_argument("invald input : " + ss.str());
    ret.push_back(elem);
  }
  return ret;
}
