---
title: "Metodologia"
description: ""
---

El Método de Diferencias Finitas (FDM) es una técnica para aproximar derivadas
que permite obtener soluciones numéricas a Ecuaciones Diferenciales Ordinarias
y Parciales.

Para implementar computacionalmente este método con el objetivo de resolver la
ecuación de calor, vamos a crear una "grilla" de puntos o "nodos". El método
busca discretizar el espacio y el tiempo de manera que haya una cantidad de
puntos en el espacio para los cuales se puedan evaluar variables de estado
(en este caso la temperatura) en el tiempo.

Basándonos en esta discretización, escribimos la aproximación de la ecuación
de calor y las derivadas propuestas por
[Powell, A. (2002)](/referencias):

$$
\frac{\partial T}{\partial t} \approx \frac{T_{i,j}^{l+1} - T_{i,j}^{l}}{\Delta t}
$$

En donde $i$ y $j$ son los índices de las posiciones y $l$ el índice de los
puntos temporales.

$$
\frac{\partial^2 T}{\partial x^2} \approx \frac{T_{i+1,j}^{l} - 2T_{i,j}^{l} + T_{i-1,j}^{l}}{(\Delta x)^2}
$$

$$
\frac{\partial^2 T}{\partial y^2} \approx \frac{T_{i,j+1}^{l} - 2T_{i,j}^{l} + T_{i,j-1}^{l}}{(\Delta y)^2}
$$

Los errores son de $o[\Delta t]$, $o[(\Delta x)^2]$ y $o[(\Delta y)^2]$
respectivamente.

Armados con estas expresiones, reemplazamos en la ecuación de calor para
obtener:

$$
\frac{T_{i,j}^{l+1} - T_{i,j}^{l}}{\Delta t} = c^2 \left[ \frac{T_{i+1,j}^{l} - 2T_{i,j}^{l} + T_{i-1,j}^{l}}{(\Delta x)^2} + \frac{T_{i,j+1}^{l} - 2T_{i,j}^{l} + T_{i,j-1}^{l}}{(\Delta y)^2} \right]
$$

Finalmente, despejando $T_{i,j}^{l+1}$ (la temperatura en la iteración
consecuente):

$$
T_{i,j}^{l+1} = T_{i,j}^{l} + c^2 \cdot \Delta t \cdot \left[ \frac{T_{i+1,j}^{l} - 2T_{i,j}^{l} + T_{i-1,j}^{l}}{(\Delta x)^2} + \frac{T_{i,j+1}^{l} - 2T_{i,j}^{l} + T_{i,j-1}^{l}}{(\Delta y)^2} \right]
$$
