#include <iostream>
#include "heatmap.hpp"

int main()
{
	int steps = 600; // Iteraciones
	double dt = 0.1; // dt por iteración

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

	// Estado después de 1 minuto
	for (int i = 1; i <= steps; i++) {
		hierro.StepFDM(dt);
	}

	// Para una placa de cobre (c = 113x10^-6) de 40x60 cm,
	// Grilla de 50 x 75 nodos, cada uno de longitud 8x10^-3 m
	Heatmap cobre(50, 75, 113e-6, 8e-3);

	// Punto caliente (100 C) de 4x4 nodos en el medio
	for (size_t i = cobre.Rows() / 2 - 4; i < cobre.Rows() / 2 + 4; i++) {
		for (size_t j = cobre.Cols() / 2 - 4; j < cobre.Cols() / 2 + 4;
		     j++) {
			cobre.Set(i, j, 393.15);
		}
	}

	// Estado después de 1 minuto
	for (int i = 1; i <= steps; i++) {
		cobre.StepFDM(dt);
	}

	// throw std::invalid_argument
	// Para este miembro, las condiciones no permiten dt
	// mayor a 0.283186s ya que se alcanza inestabilidad

	//cobre.StepFDM(0.8);

	// Note que sí se permite para el hierro, ya que
	// el criterio de inestabilidad se alcanza en:
	// dt > 1.40351s (con espaciado de este miembro)

	hierro.StepFDM(0.8);

	return 0;
}
