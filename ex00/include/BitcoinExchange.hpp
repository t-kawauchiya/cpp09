/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/18 17:30:28 by takawauc         ###   ########.fr       */
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
  BitcoinExchange(std::string filename);
  BitcoinExchange(const BitcoinExchange& other);

  ~BitcoinExchange();

  BitcoinExchange& operator=(const BitcoinExchange& other);

  std::list<std::pair<time_t, double> > getData() const;

  double getPrice(time_t date) const;
  void putData(std::pair<time_t, double> data) const;

private:
  BitcoinExchange();
  void importData(std::string filepath);
  std::list<std::pair<time_t, double> > data_;
};

std::ostream& operator<<(std::ostream& os, const BitcoinExchange& be);

int parseStringTime(std::string str, time_t& t);

#endif
