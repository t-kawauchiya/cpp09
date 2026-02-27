#!/bin/zsh
set -u

BIN=./RPN

run_test() {
  printf '[arg]'
  for a in "$@"; do
    printf ' %s' "$a"
  done
  printf '\n'

  "$BIN" "$@"
  printf '\n'
}

run_test "8 9 * 9 - 9 - 9 - 4 - 1 +"
run_test "7 7 * 7 -"
run_test "1 2 * 2 / 2 * 2 4 - +"
run_test "(1 + 1)"
