#include "tests.h"

int main() {
  int not_failed = 0;

  not_failed |= test_matrix();

  return (not_failed == 0) ? 0 : 1;
}