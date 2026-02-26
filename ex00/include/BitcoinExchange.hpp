/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/26 19:13:49 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _BITCOIN_EXCHANGE_H_
#define _BITCOIN_EXCHANGE_H_

#include <ctime>
#include <list>
#include <string>

#define DATA_FILE_PATH "./data.csv"

class BitcoinExchange {
public:
  BitcoinExchange();
  BitcoinExchange(const BitcoinExchange &other);

  ~BitcoinExchange();

  BitcoinExchange &operator=(const BitcoinExchange &other);

  std::list<std::pair<time_t, int> > getData() const;

  void addData(std::pair<time_t, int> data);
  int getPrice(time_t date) const;
  int init(std::string filepath);

private:
  std::list<std::pair<time_t, int> > data_;
};

std::ostream &operator<<(std::ostream &os, const BitcoinExchange &be);

int parseStringTime(std::string str, time_t &t);

#endif
