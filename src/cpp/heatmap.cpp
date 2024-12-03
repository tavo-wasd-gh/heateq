#include <cmath>
#include <omp.h>
#include <sstream>
#include <string>
#include <numeric>
#include "heatmap.hpp"

/* Constructor principal: Inicializa una matriz de tamaño m x n con un valor
 * constante HM_AMBIENT. */
Heatmap::Heatmap(size_t m, size_t n, double c, double d)
	: n(n) /* Número de columnas */
	, map(m * n, HM_AMBIENT) /* Inicializa std::vector<double> */
	, d(d) /* Espaciado entre nodos */
	, c(c) /* Coeficiente de difusión */
{
	/* Validación: La matriz debe tener al menos un elemento, esta
	 * validación sí es necesaria ya que std::vector permite vectores de
	 * tamaño 0, pero para efectos de este módulo esto no es viable */
	if (m == 0 || n == 0) {
		throw std::out_of_range("must have at least one element");
	}

	/* No se realizan otras validaciones ya que las limitaciones como un
	 * máximo de nodos o de memoria que puede alojar un elemento de tipo
	 * std::vector se las dejamos a la implementación de la STL del caso */
}

/* Constructor adicional:
 * Inicializa la matriz con un valor inicial de temperatura. */
Heatmap::Heatmap(size_t m, size_t n, double c, double d, double temp)
	: n(n)
	, map(m * n, temp)
	, d(d)
	, c(c)
{
	if (m == 0 || n == 0) {
		throw std::out_of_range("must have at least one element");
	}

	/* Para este caso, sí se debe agregar una comprobación de la temperatura
	 * ya que no hay otros métodos para evitar un error como este */
	if (temp < 0) {
		std::ostringstream error;
		error << "invalid temperature: " << temp << "K";
		throw std::out_of_range(error.str());
	}
}

/* Constructor de copia: Copia los datos de otro objeto Heatmap, mediante un
 * deep copy */
Heatmap::Heatmap(const Heatmap &obj)
	: n(obj.n)
	, map(obj.map)
	, d(obj.d)
	, c(obj.c)
{
}

Heatmap::~Heatmap()
{
	/* No se utilizan recursos dinámicos de memoria, por lo que no hace
	 * falta desalocar nada sino que todo se maneja en el stack. */
}

/* Operador de asignación: Copia los datos de otro Heatmap en 'this' */
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

/* Sobrecarga del operador [] para acceder a un elemento de la matriz de manera
 * segura y conveniente. */
double Heatmap::operator[](size_t i)
{
	if (i >= map.size()) {
		throw std::out_of_range("index out of range");
	}

	return map[i];
}

/* Simula un paso de tiempo utilizando diferencias finitas. */
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

	/* El código realiza una iteración sobre una matriz representada en
	 * formato 1D (prev y map) y calcula una simulación basada en un esquema
	 * de diferencias finitas en dos dimensiones.
	 * El pragma #pragma omp parallel for paraleliza el bucle del índice i,
	 * distribuyendo las filas de la matriz entre los hilos. */

#pragma omp parallel for
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

/* Retorna la matriz completa como una referencia constante, es decir, no se
 * puede modificar, pero, no aloja memoria extra y se pueden consultar
 * seguramente los elementos de la matriz o imprimir como se guste */
const std::vector<double> &Heatmap::Map()
{
	return map;
}

/* Establece un valor de temperatura en una celda específica */
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

/* Genera una representación textual de la matriz. */
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

/* Genera un informe con los parámetros de la matriz y sus valores. */
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

/* Calcula el promedio de los valores en la matriz. */
double Heatmap::Avg()
{
	return std::accumulate(map.begin(), map.end(), 0) /
	       static_cast<double>(map.size());
}
