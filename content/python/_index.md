---
title: "Implementación en Python"
description: ""
---

## Descripción del Programa

Este programa simula la ecuación de difusión de calor en 2D utilizando
diferencias finitas. Incluye un menú interactivo para configurar los parámetros
del dominio, las condiciones iniciales, y las condiciones de frontera, además de
animar la evolución de la temperatura.

## Clases y Métodos

### Clase `ecuacion_calor`

Esta clase implementa la lógica de la simulación, incluyendo:

- Definición del dominio espacial.
- Verificación de estabilidad numérica.
- Resolución de la ecuación del calor.
- Generación de animaciones.

#### Atributos

- `Lx`, `Ly`: Dimensiones del dominio en metros.
- `Nx`, `Ny`: Número de puntos en la malla para `x` e `y`.
- `dt`: Paso de tiempo.
- `c`: Constante de difusión del calor.
- `condiciones_frontera`: Diccionario con las condiciones de frontera.
- `condiciones_iniciales`: Tipo de condición inicial (e.g., punto caliente).
- `T`: Tiempo total de simulación.
- `tmax`: Temperatura máxima inicial.
- `x`, `y`: Coordenadas espaciales.
- `dx`, `dy`: Espaciado entre nodos.
- `u`: Matriz de temperatura actual.
- `u_new`: Matriz de temperatura para el siguiente paso temporal.
- `snapshots`: Lista para almacenar instantáneas de la simulación.

#### Métodos

##### `__init__(self, Lx, Ly, Nx, Ny, dt, c, condiciones_frontera, condiciones_iniciales, T, tmax)`

- **Descripción**: Inicializa el dominio, la malla y los parámetros de la simulación.
- **Validaciones**:
  - Comprueba la estabilidad de Courant (\( \alpha_x, \alpha_y < 0.5 \)).
- **Inicializa**:
  - Matrices `u` y `u_new` con ceros.
  - Lista `snapshots` para las instantáneas.

##### `definir_condiciones_iniciales()`

- **Descripción**: Aplica las condiciones iniciales en la matriz `u`.
- **Opciones**:
  - `punto_caliente`: Zona central de alta temperatura.
  - `gradiente_lineal`: Incremento lineal de temperatura.
  - `onda_sinusoidal`: Distribución sinusoidal en el dominio.
  - `uniforme`: Temperatura constante en todo el dominio.

##### `definir_condiciones_de_frontera()`

- **Descripción**: Aplica las condiciones de frontera (Dirichlet o Neumann) en las cuatro caras del dominio.
- **Detalles**:
  - `Dirichlet`: Valores específicos de temperatura.
  - `Neumann`: Derivada normal igual a cero (aislamiento).

##### `resolucion_ec_calor()`

- **Descripción**: Resuelve la ecuación de calor usando diferencias finitas explícitas.
- **Proceso**:
  - Itera sobre los pasos temporales `Nt`.
  - Calcula la nueva temperatura `u_new` con el esquema explícito.
  - Aplica las condiciones de frontera en cada paso.
  - Copia `u_new` a `u` para el siguiente paso.
  - Guarda instantáneas cada \( \frac{1}{100} \) del total.

##### `animar()`

- **Descripción**: Crea una animación de la evolución de la temperatura.
- **Detalles**:
  - Usa `matplotlib.animation.FuncAnimation` para visualizar las instantáneas.
  - Muestra un mapa de calor (`cmap='hot'`).

#### Funciones del Menú

##### `menu()`

- **Descripción**: Imprime el menú principal y solicita una opción al usuario.

- **Opciones**:
  1. Configurar parámetros del dominio.
  2. Configurar condiciones de frontera.
  3. Configurar condición inicial.
  4. Resolver y animar la ecuación del calor.
  5. Salir.

##### `configurar_dominio()`

- **Descripción**: Solicita al usuario las dimensiones, número de nodos, paso de tiempo, constante de difusión, tiempo máximo de simulación y temperatura máxima inicial.
- **Retorno**:
  - Diccionario con las configuraciones del dominio.

##### `configurar_condiciones_frontera()`

- **Descripción**: Solicita al usuario las condiciones de frontera (Dirichlet o Neumann) para los lados izquierdo, derecho, inferior y superior.
- **Retorno**:
  - Diccionario con las configuraciones de frontera.

##### `configurar_condicion_inicial()`

- **Descripción**: Solicita al usuario seleccionar una condición inicial de las opciones disponibles (punto caliente, gradiente lineal, onda sinusoidal, uniforme).
- **Retorno**:
  - Tipo de condición inicial.

### Flujo Principal (`main()`)

##### Descripción

Controla la ejecución del programa interactivo, permitiendo al usuario:

1. Configurar los parámetros del dominio.
2. Configurar las condiciones de frontera.
3. Configurar la condición inicial.
4. Resolver la ecuación de calor y visualizar la animación.

##### Detalles
- Comprueba que todas las configuraciones estén completas antes de ejecutar la simulación.
- Crea una instancia de la clase `ecuacion_calor` con los parámetros proporcionados.
- Invoca los métodos de resolución y animación.

##### Salida

- Visualización de la evolución de la temperatura en un mapa de calor animado.
