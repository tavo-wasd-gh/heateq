---
title: "Inicio"
description: "Métodos para resolver numéricamente la ecuación de calor en dos dimensiones"
---

# Ecuación de calor en 2D

![](/heateq/heat.gif)

Suponga que $T = T(x, y, t)$ es una variable escalar que define la temperatura
de una región de dos dimensiones en el plano Cartesiano $(x, y)$ como función
del tiempo. Bajo condiciones ideales (sin fuentes de energía externas, capacidad
calórica uniforme, aislamiento perfecto), la ecuación de calor está dada por:

$$
\frac{\partial T}{\partial t} =
    c^2 \left[
        \frac{\partial^2 T}{\partial x^2} + \frac{\partial^2 T}{\partial y^2}
    \right]
$$

Para alguna región acotada $x \in [0, a]$ y $y \in [0, b]$. Las condiciones de
frontera pueden cambiar y la dinámica está determinada por éstas y por las
condiciones iniciales.
