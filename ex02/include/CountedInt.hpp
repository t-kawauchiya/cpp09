/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CountedInt.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/02/28 23:21:29 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _COUNTED_INT_H_
#define _COUNTED_INT_H_

class CountedInt {
public:
  CountedInt();
  CountedInt(int v);
  CountedInt(const CountedInt& other);

  ~CountedInt();

  CountedInt& operator=(const CountedInt& other);

  int value() const;

  static void reset_cnt();
  static int get_cnt();
  bool operator<(const CountedInt& rhs) const;
  bool operator<=(const CountedInt& rhs) const;
  bool operator>(const CountedInt& rhs) const;
  bool operator>=(const CountedInt& rhs) const;
  bool operator==(const CountedInt& rhs) const;
  bool operator!=(const CountedInt& rhs) const;

private:
  int v_;
  static int cnt_;
};

#endif
