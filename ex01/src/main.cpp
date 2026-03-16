/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:56:41 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/16 22:28:56 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <cstdlib>
#include <ios>
#include <iostream>
#include <sstream>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

int solve(std::string str);

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cerr << "Error: RPN needs one arg." << std::endl;
    return EXIT_FAILURE;
  }
  if (argc != 2) {
    std::cerr << "Error: RPN can not accept multiple args." << std::endl;
    return EXIT_FAILURE;
  }
  try {
    std::cout << solve(argv[1]) << std::endl;
  } catch (std::runtime_error e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

int solve(std::string str) {
  std::istringstream iss(str);
  RPN rpn;
  std::string item;

  while (std::getline(iss, item, ' ')) {
    // std::cout << "rpn: " << rpn << ", ";
    // std::cout << "item: " << item << std::endl;
    rpn.acceptToken(item);
  }
  if (rpn.getStack().size() != 1)
    throw std::invalid_argument("bad input : " + str);
  return rpn.getTop();
}
