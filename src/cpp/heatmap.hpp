#ifndef HEATMAP_HPP
#define HEATMAP_HPP

#include <cstddef>
#include <vector>
#include <string>

class Heatmap
{
public:
  // Default
  Heatmap() = delete;

  // 2D
  Heatmap(size_t rows, size_t cols, double diffusivity, double res);

  // Copy
  Heatmap(const Heatmap &obj);

  // Assign
  Heatmap& operator=(const Heatmap &obj);

  // Subscript
  double operator[](size_t i);

  ~Heatmap();

  size_t Size();

  void Set(size_t index, double value);

  std::string Display();

  std::string Report();

  // Simulation
  void FDM(double dt);

private:
  size_t n;
  std::vector<double> map;
  double res;
  double c;
};

#endif // HEATMAP_HPP
