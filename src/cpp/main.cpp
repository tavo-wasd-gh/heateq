#include <cstddef>
#include <iostream>
#include <cmath>
#include "heatmap.hpp"

int main() {
  size_t rows = 50;
  size_t cols = 75;
  double c = 113e-6;
  double res = 1e-4;

  Heatmap placa(rows, cols, c, res);

  for (size_t i = 24; i < 26; i++) {
    for (size_t j = 36; j < 39; j++) {
      placa.Set((i * cols) + j, 373.15);
    }
  }

  for (int i = 1; i <= 400; i++) {
    placa.FDM(0.1);
  }

  std::cout << placa.Display() << std::endl;

  return 0;
}
