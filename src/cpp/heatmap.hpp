#ifndef HEATMAP_HPP
#define HEATMAP_HPP

#include <string>
#include <vector>

#ifndef HM_AMBIENT
#define HM_AMBIENT 293.15
#endif

class Heatmap {
    public:
	// Default
	Heatmap() = delete;

	// 2D
	Heatmap(size_t m, size_t n, double c, double d);
	Heatmap(size_t m, size_t n, double c, double d, double temp);

	// Copy
	Heatmap(const Heatmap &obj);

	// Assign
	Heatmap &operator=(const Heatmap &obj);

	// Subscript
	double operator[](size_t i);

	~Heatmap();

	size_t Size();

	void Set(size_t i, size_t j, double value);

	std::string Display();

	std::string Report();

	// Simulation
	void StepFDM(double dt);

    private:
	size_t n; // N dimension of MxN mesh
	std::vector<double> map; // mesh matrix
	double d; // node length
	double c; // (material) diffusivity
};

#endif // HEATMAP_HPP
