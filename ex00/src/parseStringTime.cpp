/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseStringTime.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:33:37 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/26 18:34:00 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

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
