#include <cmath>
#include <sstream>
#include <string>
#include <numeric>
#include "heatmap.hpp"

Heatmap::Heatmap(size_t m, size_t n, double c, double d)
	: n(n)
	, map(m * n, HM_AMBIENT)
	, d(d)
	, c(c)
{
	if (m == 0 || n == 0) {
		throw std::out_of_range("must have at least one element");
	}
}

Heatmap::Heatmap(size_t m, size_t n, double c, double d, double temp)
	: n(n)
	, map(m * n, temp)
	, d(d)
	, c(c)
{
	if (m == 0 || n == 0) {
		throw std::out_of_range("must have at least one element");
	}

	if (temp < 0) {
		std::ostringstream error;
		error << "invalid temperature: " << temp << "K";
		throw std::out_of_range(error.str());
	}
}

Heatmap::Heatmap(const Heatmap &obj)
	: n(obj.n)
	, map(obj.map)
	, d(obj.d)
	, c(obj.c)
{
}

Heatmap::~Heatmap()
{
	// No heap allocation
}

Heatmap &Heatmap::operator=(const Heatmap &obj)
{
	if (this != &obj) {
		n = obj.n;
		map = obj.map;
		d = obj.d;
		c = obj.c;
	}
	return *this;
}

double Heatmap::operator[](size_t i)
{
	if (i >= map.size()) {
		throw std::out_of_range("index out of range");
	}

	return map[i];
}

void Heatmap::StepFDM(double dt)
{
	if (dt > pow(d, 2) / (2 * c)) {
		std::ostringstream error;
		error << "time step dt = " << dt
		      << " exceeds stability limit: ";
		error << "dt <= " << pow(d, 2) / (2 * c);
		throw std::invalid_argument(error.str());
	}

	size_t m = map.size() / n;

	Heatmap prev = *this;

	for (size_t i = 1; i < m - 1; ++i) {
		for (size_t j = 1; j < n - 1; ++j) {
			double ddT_x =
				(prev[(i + 1) * n + j] - 2 * prev[i * n + j] +
				 prev[(i - 1) * n + j]) /
				pow(d, 2);
			double ddT_y =
				(prev[i * n + (j + 1)] - 2 * prev[i * n + j] +
				 prev[i * n + (j - 1)]) /
				pow(d, 2);
			map[i * n + j] =
				prev[i * n + j] + c * dt * (ddT_x + ddT_y);
		}
	}
}

size_t Heatmap::Size()
{
	return map.size();
}

size_t Heatmap::Rows()
{
	return map.size() / n;
}

size_t Heatmap::Cols()
{
	return n;
}

const std::vector<double> &Heatmap::Map()
{
	return map;
}

void Heatmap::Set(size_t i, size_t j, double temp)
{
	if (i >= map.size() / n || j >= n) {
		throw std::out_of_range("index out of range");
	}

	if (temp < 0) {
		std::ostringstream error;
		error << "invalid temperature: " << temp << "K";
		throw std::out_of_range(error.str());
	}

	map[i * n + j] = temp;
}

std::string Heatmap::Display()
{
	size_t size = map.size();

	std::ostringstream output;

	output << "[";
	for (size_t i = 0; i < size / n; i++) {
		if (i != 0) {
			output << " [";
		} else {
			output << "[";
		}
		for (size_t j = 0; j < n; j++) {
			output << map[(i * n) + j];
			if (j != n - 1) {
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

	size_t size = map.size();

	for (size_t i = 0; i < size; ++i) {
		if (i != size - 1) {
			report << map[i] << ",";
		} else {
			report << map[i];
		}
	}

	return report.str();
}

double Heatmap::Avg()
{
	return std::accumulate(map.begin(), map.end(), 0) /
	       static_cast<double>(map.size());
}
