/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/28 23:27:35 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CountedInt.hpp"
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
    CountedInt().reset_cnt();
    std::vector<int> input = parseArgsToVector(argc, argv);
    PmergeMe pmm(input);
    pmm.solve();
    std::cout << "\n=====result=====\n";
    std::vector<int> res = pmm.getResult();
    for (std::vector<int>::const_iterator it = res.begin(); it != res.end();
         it++)
      std::cout << *it << " ";
    std::cout << "\ncomp count: " << CountedInt().get_cnt() << std::endl;
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
