/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/06/12 19:43:31 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <time.h>

std::vector<int> parseStrArrToVector(char** sarr, int size);

int getDuration(clock_t start, clock_t end);

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cerr << "Error: PmergeMe needs args." << std::endl;
    return EXIT_FAILURE;
  }
  try {
    std::vector<int> input = parseStrArrToVector(argv + 1, argc - 1);
    PmergeMe pmm(input);

    clock_t start_v = clock();
    pmm.solve_v();
    clock_t end_v = clock();

    clock_t start_d = clock();
    pmm.solve_d();
    clock_t end_d = clock();

    std::cout << "Before: " << input << std::endl;
    std::cout << "After:  " << pmm.getResult_v() << std::endl;
    std::cout << "After:  " << pmm.getResult_d() << std::endl;
    std::cout << "Time to process a range of " << input.size()
              << " elements with std::vector : " << std::fixed
              << getDuration(start_v, end_v) << " us\n";
    std::cout << "Time to process a range of " << input.size()
              << " elements with std::deque  : " << std::fixed
              << getDuration(start_d, end_d) << " us\n";
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

int getDuration(clock_t start, clock_t end) {
  return (end - start  * 1000 * 1000 / CLOCKS_PER_SEC);
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
