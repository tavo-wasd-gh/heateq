#include <iostream>
#include "heatmap.hpp"

int main()
{
	int steps = 400; // Iteraciones
	double dt = 0.01; // dt por iteración

	// Para una placa de hierro (c = 22.8x10^-6 m^s/s) de 40x60 cm,
	// Grilla de 50 x 75 nodos, cada uno de longitud 8x10^-3 m
	Heatmap hierro(50, 75, 22.8e-6, 8e-3);

	// Punto caliente (100 C) de 4x4 nodos en el medio
	for (size_t i = hierro.Rows() / 2 - 4; i < hierro.Rows() / 2 + 4; i++) {
		for (size_t j = hierro.Cols() / 2 - 4;
		     j < hierro.Cols() / 2 + 4; j++) {
			hierro.Set(i, j, 393.15);
		}
	}

	// Estado después de 4s
	for (int i = 1; i <= steps; i++) {
		hierro.StepFDM(dt);
	}

	// El miembro &Map() permite accesar de manera
	// segura el mapa del objeto, sin modificarlo
	const std::vector<double> &map = hierro.Map();

	//map[0] = 0.0; // error: assignment of read-only location
	//std::cout << map[0] << std::endl; // Sí permite consultar

	// Vamos a crear un reporte para graficarlo con el paso
	// del tiempo, para esto, necesitamos imprimir un reporte
	// de cada grilla con su índice temporal, y los datos
	// utilizados para calcularla. Esta funcionalidad
	// se encuentra en el miembro Report()

	// Placa de cobre (c = 113x10^-6) de 40x60 cm,
	Heatmap cobre(50, 75, 113e-6, 8e-3);

	// Punto caliente (100 C) de 4x4 nodos en el medio
	for (size_t i = cobre.Rows() / 2 - 4; i < cobre.Rows() / 2 + 4; i++) {
		for (size_t j = cobre.Cols() / 2 - 4; j < cobre.Cols() / 2 + 4;
		     j++) {
			cobre.Set(i, j, 393.15);
		}
	}

	// Estado luego de condiciones iniciales
	double time = 0.0;
	//std::cout << time << "," << cobre.Report() << std::endl;
	// Estado después de 4s
	for (int i = 1; i <= steps; i++) {
		cobre.StepFDM(dt);
		time += dt;
		//std::cout << time << "," << cobre.Report() << std::endl;
	}

	// Para este miembro, las condiciones no permiten dt
	// mayor a 0.283186s ya que se alcanza inestabilidad

	//cobre.StepFDM(0.8); // throw std::invalid_argument

	// Note que sí se permite para el hierro, ya que
	// el criterio de inestabilidad se alcanza en:
	// dt > 1.40351s (con espaciado de este miembro)

	hierro.StepFDM(0.8);

	// Ahora, creamos una visualización interesante
	Heatmap oro(50, 96, 127e-6, 8e-3);

	double dt_au = 0.01;
	double time_oro = 0.0;
	int steps_au = 400; // Iteraciones

	for (int n = 1; n <= steps_au; n++) {
		if (n < 40) {
			oro.Set(5 + 40 * n / 40, 4, 393.15);
			oro.Set(5 + 40 * n / 40, 5, 393.15);
		}

		if (n < 60 && n > 20) {
			oro.Set(5 + 36 * (n - 20) / 40, 19, 393.15);
			oro.Set(5 + 35 * (n - 20) / 40, 20, 393.15);
		}

		if (n < 80 && n > 40) {
			oro.Set(23, 3 + 23 * (n - 40) / 40, 393.15);
			oro.Set(24, 3 + 22 * (n - 40) / 40, 393.15);
		}

		if (n < 120 && n > 80) {
			oro.Set(5, 27 + 21 * (n - 80) / 40, 393.15);
			oro.Set(6, 28 + 19 * (n - 80) / 40, 393.15);
		}

		if (n < 120 && n > 80) {
			oro.Set(5 + 18 * (n - 80) / 40, 29 + 11 * (n - 80) / 40,
				393.15);
			oro.Set(5 + 17 * (n - 80) / 40, 30 + 11 * (n - 80) / 40,
				393.15);
			oro.Set(5 + 16 * (n - 80) / 40, 31 + 11 * (n - 80) / 40,
				393.15);
		}

		if (n < 160 && n > 120) {
			oro.Set(20 + 20 * (n - 120) / 40,
				40 - 11 * (n - 120) / 40, 393.15);
			oro.Set(19 + 19 * (n - 120) / 40,
				41 - 11 * (n - 120) / 40, 393.15);
			oro.Set(18 + 18 * (n - 120) / 40,
				42 - 11 * (n - 120) / 40, 393.15);
		}

		if (n < 200 && n > 160) {
			oro.Set(37, 28 + 26 * (n - 160) / 40, 393.15);
			oro.Set(38, 29 + 22 * (n - 160) / 40, 393.15);
		}

		if (n < 210 && n > 170) {
			oro.Set(5 + 41 * (n - 170) / 40,
				58 - 12 * (n - 170) / 40, 393.15);
			oro.Set(5 + 39 * (n - 170) / 40,
				57 - 12 * (n - 170) / 40, 393.15);
			oro.Set(5 + 40 * (n - 170) / 40,
				56 - 12 * (n - 170) / 40, 393.15);
		}

		if (n < 210 && n > 170) {
			oro.Set(5 + 41 * (n - 170) / 40,
				58 + 12 * (n - 170) / 40, 393.15);
			oro.Set(5 + 39 * (n - 170) / 40,
				57 + 12 * (n - 170) / 40, 393.15);
			oro.Set(5 + 40 * (n - 170) / 40,
				56 + 12 * (n - 170) / 40, 393.15);
		}

		if (n < 240 && n > 200) {
			oro.Set(6 + 36 * (n - 200) / 40, 79, 393.15);
			oro.Set(7 + 37 * (n - 200) / 40, 78, 393.15);
			oro.Set(8 + 38 * (n - 200) / 40, 77, 393.15);
		}

		if (n < 260 && n > 220) {
			oro.Set(6, 64 + 25 * (n - 220) / 40, 393.15);
			oro.Set(7, 68 + 24 * (n - 220) / 40, 393.15);
			oro.Set(8, 72 + 23 * (n - 220) / 40, 393.15);
		}

		oro.StepFDM(dt_au);
		time_oro += dt_au;
		std::cout << time_oro << "," << oro.Report() << std::endl;
	}

	return 0;
}
