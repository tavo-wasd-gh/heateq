#include <iostream>
#include "heatmap.hpp"

int main()
{
	size_t rows = 50;
	size_t cols = 75;
	double alpha = 22.8e-6;
	double res = 8e-3;

	Heatmap placa(rows, cols, alpha, res);

	for (size_t i = 21; i < 29; i++) {
		for (size_t j = 33; j < 41; j++) {
			placa.Set(i, j, 393.15);
		}
	}

	for (int i = 1; i <= 600; i++) {
		placa.StepFDM(0.1);
	}

	std::cout << placa.Display() << std::endl;

	return 0;
}
