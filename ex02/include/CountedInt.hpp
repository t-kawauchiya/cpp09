/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CountedInt.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takawauc <takawauc@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:57:27 by takawauc          #+#    #+#             */
/*   Updated: 2026/03/07 00:39:39 by takawauc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _COUNTED_INT_H_
#define _COUNTED_INT_H_

class CountedInt {
public:
  CountedInt(int v);
  CountedInt(const CountedInt& other);
  ~CountedInt();

  int getValue() const;
  static int getCnt();

  bool operator>(const CountedInt& rhs) const;

private:
  CountedInt();
  CountedInt& operator=(const CountedInt& other);
  int v_;
  static int cnt_;
};

#endif
