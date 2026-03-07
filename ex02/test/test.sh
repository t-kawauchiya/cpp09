#!/bin/zsh
set -u

BIN=./PmergeMe

# run_test() {
#   printf '[arg]'
#   for a in "$@"; do
#     printf ' %s' "$a"
#   done
#   printf '\n'
#
#   "$BIN" "$@"
#   printf '\n'
# }

run_test() {
  local max=5
  local count=$#

  printf '[arg]'

  local i=0
  for a in "$@"; do
    if (( i < max )); then
      printf ' %s' "$a"
    else
      printf ' ...'
      break
    fi
    ((i++))
  done

  if (( count > max )); then
    printf ' (%d args)' "$count"
  fi
  printf '\n'

  "$BIN" "$@"
  printf '\n'
}



run_test 3
run_test 1 2
run_test 2 1
run_test $(shuf -i 1-3 -n 3 | tr "\n" " ")
run_test $(shuf -i 1-4 -n 4 | tr "\n" " ")
run_test $(shuf -i 1-5 -n 5 | tr "\n" " ")
run_test $(shuf -i 1-6 -n 6 | tr "\n" " ")
run_test $(shuf -i 1-7 -n 7 | tr "\n" " ")
run_test $(shuf -i 1-8 -n 8 | tr "\n" " ")
run_test $(shuf -i 1-9 -n 9 | tr "\n" " ")
run_test $(shuf -i 1-10 -n 10 | tr "\n" " ")
run_test $(shuf -i 1-11 -n 11 | tr "\n" " ")
run_test $(shuf -i 1-12 -n 12 | tr "\n" " ")
run_test $(shuf -i 1-13 -n 13 | tr "\n" " ")
run_test $(shuf -i 1-14 -n 14 | tr "\n" " ")
run_test $(shuf -i 1-15 -n 15 | tr "\n" " ")
run_test $(shuf -i 1-16 -n 16 | tr "\n" " ")
run_test $(shuf -i 1-17 -n 17 | tr "\n" " ")
run_test $(shuf -i 1-18 -n 18 | tr "\n" " ")
run_test $(shuf -i 1-19 -n 19 | tr "\n" " ")
run_test $(shuf -i 1-20 -n 20 | tr "\n" " ")
run_test $(shuf -i 1-21 -n 21 | tr "\n" " ")
run_test $(shuf -i 1-100000 -n 3000 | tr "\n" " ")
