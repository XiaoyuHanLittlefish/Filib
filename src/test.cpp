#include <iostream>

#include "magnum/timer.h"

void func() { std::cout << "here" << std::endl; }

int main() {
  timer::Timer limited_timer(5);
  limited_timer.start(100, func);
  getchar();
}