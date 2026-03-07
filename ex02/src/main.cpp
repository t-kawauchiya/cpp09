/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/07 18:13:31 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <time.h>

std::vector<int> parseArgsToVector(int argc, char** argv);

double calcTime(clock_t start, clock_t end);

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cerr << "Error: PmergeMe needs args." << std::endl;
    return EXIT_FAILURE;
  }
  try {
    std::vector<int> input = parseArgsToVector(argc, argv);
    PmergeMeVector pmmv(input);
    PmergeMeDeque pmmd(input);

    clock_t start_v = clock();
    pmmv.solve();
    clock_t end_v = clock();

    CountedInt::resetCnt();
    clock_t start_d = clock();
    pmmd.solve();
    clock_t end_d = clock();

    std::cout << "Before: " << input << std::endl;
    std::cout << "After:  " << pmmv.getResult() << std::endl;
    std::cout << "After:  " << pmmd.getResult() << std::endl;
    std::cout << "Time to process a range of " << input.size()
              << " elements with std::vector : " << std::fixed
              << std::setprecision(5) << calcTime(start_v, end_v) << "us\n";
    std::cout << "Time to process a range of " << input.size()
              << " elements with std::deque  : " << std::fixed
              << std::setprecision(5) << calcTime(start_d, end_d) << "us\n";
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

double calcTime(clock_t start, clock_t end) {
  return static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

std::vector<int> parseArgsToVector(int argc, char** argv) {
  std::vector<int> ret;
  for (int i = 1; i < argc; i++) {
    std::stringstream ss(argv[i]);
    int elem;
    ss >> elem;
    if (!ss)
      throw std::runtime_error("invald input : " + ss.str());
    ret.push_back(elem);
  }
  return ret;
}
