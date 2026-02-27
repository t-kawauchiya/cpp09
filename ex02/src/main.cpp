/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/27 23:39:37 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

std::vector<int> parseArgsToVector(int argc, char** argv);

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cerr << "Error: PmergeMe needs args." << std::endl;
    return EXIT_FAILURE;
  }
  try {
    std::vector<int> input = parseArgsToVector(argc, argv);
    PmergeMe pmm(input);
    // std::cout << pmm;
    pmm.solve();
    // std::cout << "\n=====result=====\n" << pmm.getResult() << std::endl;
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
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
