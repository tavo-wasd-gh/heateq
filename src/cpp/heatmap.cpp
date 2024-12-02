#include <cmath>
#include <sstream>
#include <string>
#include "heatmap.hpp"

Heatmap::Heatmap(size_t rows, size_t cols, double diff, double res)
	: n(cols)
	, d(res)
	, c(diff)
{
	if (cols == 0 || rows == 0) {
		throw std::out_of_range("Must have at least one element");
	}

	size_t size = rows * cols;
	if (size / rows != cols) {
		throw std::overflow_error(
			"Total elements exceeds maximum 'size_t' value");
	}

	map = std::vector<double>(size, HM_AMBIENT);
}

Heatmap::Heatmap(size_t rows, size_t cols, double diff, double res, double temp)
	: n(cols)
	, d(res)
	, c(diff)
{
	if (cols == 0 || rows == 0) {
		throw std::out_of_range("Must have at least one element");
	}

	size_t size = rows * cols;
	if (size / rows != cols) {
		throw std::overflow_error(
			"Total elements exceeds maximum 'size_t' value");
	}

	map = std::vector<double>(size, temp);
}

Heatmap::Heatmap(const Heatmap &obj)
{
	n = obj.n;
	map = obj.map;
	d = obj.d;
	c = obj.c;
}

Heatmap &Heatmap::operator=(const Heatmap &obj)
{
	n = obj.n;
	map = obj.map;
	d = obj.d;
	c = obj.c;
	return *this;
}

double Heatmap::operator[](size_t index)
{
	if (index >= map.size()) {
		throw std::out_of_range("Index out of range");
	}

	return map[index];
}

Heatmap::~Heatmap()
{
	// No heap allocation
}

void Heatmap::StepFDM(double dt)
{
	if (dt > pow(d, 2) / (2 * c)) {
		throw std::invalid_argument(
			"Time step exceeds stability limit");
	}

	size_t size = map.size();
	size_t rows = size / n;
	size_t cols = n;

	Heatmap prev = *this;

	for (size_t i = 1; i < rows - 1; ++i) {
		for (size_t j = 1; j < cols - 1; ++j) {
			double ddT_x = (prev[(i + 1) * cols + j] -
					2 * prev[i * cols + j] +
					prev[(i - 1) * cols + j]) /
				       pow(d, 2);
			double ddT_y = (prev[i * cols + (j + 1)] -
					2 * prev[i * cols + j] +
					prev[i * cols + (j - 1)]) /
				       pow(d, 2);
			map[i * cols + j] =
				prev[i * cols + j] + c * dt * (ddT_x + ddT_y);
		}
	}
}

size_t Heatmap::Size()
{
	return map.size();
}

void Heatmap::Set(size_t index, double value)
{
	if (index >= map.size()) {
		throw std::out_of_range("Index out of range");
	}

	map[index] = value;
}

std::string Heatmap::Display()
{
	size_t size = map.size();
	size_t rows = size / n;
	size_t cols = n;

	std::ostringstream output;

	output << "[";
	for (size_t i = 0; i < rows; i++) {
		if (i != 0) {
			output << " [";
		} else {
			output << "[";
		}
		for (size_t j = 0; j < cols; j++) {
			output << map[(i * cols) + j];
			if (j != cols - 1) {
				output << ", ";
			} else if ((i + 1) * (j + 1) == size) {
				output << "]";
			} else {
				output << "]," << std::endl;
			}
		}
	}
	output << "]";

	return output.str();
}

std::string Heatmap::Report()
{
	std::ostringstream report;

	report << c << "," << n << "," << d << ",";

	for (size_t i = 0; i < map.size(); ++i) {
		report << map[i] << ",";
	}

	return report.str();
}
