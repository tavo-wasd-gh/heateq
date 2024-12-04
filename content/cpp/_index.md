---
title: "Implementación en C++"
description: ""
---

## Descripción de la Implementación

Esta implementación simula la difusión de calor en distintas placas de
material utilizando el método de diferencias finitas para resolver la
ecuación del calor en 2D. Además, aprovecha OpenMP para paralelizar los
cálculos y mejorar la eficiencia en sistemas con varios hilos a disposición.

## Estructura del Código

---

### Clase `Heatmap`: Constructores

La clase Heatmap representa una malla 2D para simular la propagación del calor
en un material, con miembros que representan las propiedades del sistema como
su difusividad térmica. Cada celda de la malla tiene un valor que indica
su temperatura en un instante dado. La clase proporciona funcionalidades para:

- Inicializar la malla con valores específicos.
- Simular pasos de tiempo utilizando diferencias finitas.
- Acceder y modificar elementos individuales.
- Generar representaciones en texto y reportes para análisis posterior.

---

#### `Heatmap(size_t m, size_t n, double c, double d)` 

Retorna: `Heatmap obj`

Este constructor crea una instancia de un sistema a temperatura ambiente
en todos sus nodos, para establecer una temperatura distina, utilice el
siguiente método:

**Ejemplo:**

```cpp
/* Para una placa de hierro (c = 22.8x10^-6 m^s/s) de 40x60 cm,
 * Grilla de 50 x 75 nodos, cada uno de longitud 8x10^-3 m */
Heatmap hierro(50, 75, 22.8e-6, 8e-3);
```

---

#### `Heatmap(size_t m, size_t n, double c, double d, double temp)`

Retorna: `Heatmap obj`

Crea una instancia de un objeto `Heatmap` pero con una temperatura inicial.

**Ejemplo:**

```cpp
/* Placa de cobre (c = 113x10^-6) de 40x60 cm, a 10C sobre ambiente */
Heatmap cobre(50, 75, 113e-6, 8e-3, 303.15);
```

---

#### `Heatmap(const Heatmap &obj)`

Retorna: `Heatmap obj`

Este constructor de "copia profunda" crea un nuevo objeto utilizando los
parámetros el objeto argumento.

**Ejemplo:**

```cpp
Heatmap cobre_2 = cobre;
```

---

### Clase `Heatmap`: Operaciones

En este caso, la operación que tiene sentido para esta implementación es
la de asignación y también la sobrecarga del operador subscript (`[]`),
esto dado a que se utilizan para igualar las condiciones de una grilla a
otra o para accesar de manera segura el índice requerido del vector.

---

#### `operator=(const Heatmap &obj)`

Retorna: `Heatmap &obj`

Operador de asignación que copia los datos de un objeto `Heatmap` existente
al objeto actual.

**Ejemplo:**

```cpp
/* ... modificaciones a cobre_2 */
cobre = cobre_2;
/* ... ahora el objeto cobre es cobre_2 */
```

---

#### `operator[](size_t i)`

Retorna: `double`

Sobrecarga del operador de acceso que permite obtener un valor de la matriz
representada en un vector 1D.

**Ejemplo:**

```cpp
std::cout <<
    "Primer elemento de la grilla de cobre: " <<
    cobre[1] <<
    std::endl;
```

---

#### `StepFDM(double dt)`

Retorna: `void`

Simula un paso de tiempo `dt` en la malla utilizando diferencias finitas.
Este método está preparado para utilizar varios hilos si se compila con la
bandera `-fopenmp`.

La justificación para realizar este procedimiento es que para calcular los
pasos en el tiempo se requieren hacer modificaciones rápidas a una copia del
vector, la cual se guarda en el índice correspondiente. Entonces, sin importar
el orden de los hilos siempre se va a obtener el mismo resultado. Además, al
ser operaciones muy sencillas, dividirlas entre varios hilos mejora el tiempo
requerido considerablemente.

**Ejemplo:**

```cpp
/* 100 iteraciones de dt = 0.01s,
 * 10s totales de simulación */
for (int i = 1; i <= 100; i++) {
    cobre.StepFDM(0.01);
}
```

Note que, al requerir el estado anterior para calcular el siguiente, no se
puede paralelizar esta parte del bloque, de manera que la forma más conveniente
de optimizar el proceso es agilizar lo máximo posible la función `StepFDM()`.

---

#### `Size()`

Retorna: `size_t`

Retorna el tamaño total de la matriz (número de nodos).

**Ejemplo:**

```cpp
std::cout <<
    "Tamaño del vector: " <<
    cobre.Size() <<
    std::endl;
```

---

#### `Rows()`

Retorna: `size_t`

Retorna el número de filas de la matriz.

**Ejemplo:**

```cpp
std::cout <<
    "Filas de la grilla: " <<
    cobre.Rows() <<
    std::endl;
```

---

#### `Cols()`

Retorna: `size_t`

Retorna el número de columntas de la matriz.

**Ejemplo:**

```cpp
std::cout <<
    "Columnas de la grilla: " <<
    cobre.Cols() <<
    std::endl;
```

---

#### `Map()`

Retorna: `const std::vector<double>&`

Proporciona una referencia constante al vector subyacente que representa
la matriz.

**Ejemplo:**

```cpp
const std::vector<double> &map = hierro.Map();
```

---

#### `Set(size_t i, size_t j, double value)`

Retorna: `void`

Establece un valor de temperatura en una celda específica `(i, j)`.

**Ejemplo:**

```cpp
/* Punto caliente de 4x4 nodos en el medio de una placa de hierro */
for (size_t i = hierro.Rows() / 2 - 4; i < hierro.Rows() / 2 + 4; i++) {
	for (size_t j = hierro.Cols() / 2 - 4;
	     j < hierro.Cols() / 2 + 4; j++) {
		hierro.Set(i, j, 393.15);
	}
}
```

---

#### `Display()`

Retorna: `std::string`

Genera una representación textual de la matriz en formato humanamente legible.

**Ejemplo:**

```cpp
hierro.Display;
```

---

#### `Report()`

Retorna: `std::string`

Genera un informe detallado con los parámetros de la simulación y los
valores actuales de la matriz, separado por comas por conveniencia.

**Ejemplo:**

```cpp
/* Estado después de 4s */
for (int i = 1; i <= 400; i++) {
	cobre.StepFDM(0.01);
	time += dt;
	std::cout << time << "," << cobre.Report() << std::endl;
}
/* Este resultado se podría utilizar para graficar el comportamiento con el
 * software a convenir. */
```

---

#### `Avg()`

Retorna: `double`

Calcula el promedio de los valores de la matriz.

**Ejemplo:**

```cpp
std::cout << cobre.Avg() << std::endl;
```
