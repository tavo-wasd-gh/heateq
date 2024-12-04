#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


class ecuacion_calor:
    def __init__(self, Lx, Ly, Nx, Ny, dt, c, condiciones_frontera, condiciones_iniciales, T, tmax):
        self.Lx = Lx
        self.Ly = Ly
        self.Nx = Nx
        self.Ny = Ny
        self.dt = dt
        self.c = c
        self.T = T
        self.tmax = tmax
        self.condiciones_frontera = condiciones_frontera
        self.condiciones_iniciales = condiciones_iniciales

        # Crear la malla espacial
        self.x = np.linspace(0, Lx, Nx)
        self.y = np.linspace(0, Ly, Ny)
        self.dx = self.x[1] - self.x[0]
        self.dy = self.y[1] - self.y[0]

        # Verificar estabilidad de Courant
        self.alpha_x = c**2 * dt / self.dx**2
        self.alpha_y = c**2 * dt / self.dy**2
        if self.alpha_x >= 0.5 or self.alpha_y >= 0.5:
            raise ValueError("Condición de estabilidad violada: reduce dt o incrementa Nx/Ny.")

        # Tiempo total de simulación
        self.Nt = int(T / dt)

        # Inicialización de la malla de temperatura
        self.u = np.zeros((Nx, Ny))  # Temperatura actual
        self.u_new = np.zeros((Nx, Ny))  # Temperatura siguiente

        # Almacenar instantáneas para la animación
        self.snapshots = []

    def definir_condiciones_iniciales(self):
        """
        Establece la condición inicial de temperatura en el dominio.
        """
        if self.condiciones_iniciales == 'punto_caliente':
            cx, cy = self.Nx // 2, self.Ny // 2
            self.u[cx-5:cx+5, cy-5:cy+5] = self.tmax
        elif self.condiciones_iniciales == 'gradiente_lineal':
            self.u[:, :] = np.linspace(0, self.tmax, self.Nx)[:, None]
        elif self.condiciones_iniciales == 'onda_sinusoidal':
            X, Y = np.meshgrid(self.x, self.y, indexing='ij')
            self.u = self.tmax * np.sin(np.pi * X / self.Lx) * np.sin(np.pi * Y / self.Ly)
        elif self.condiciones_iniciales == 'uniforme':
            self.u[:, :] = self.tmax

    def definir_condiciones_de_frontera(self):
        """
        Aplica las condiciones de frontera.
        """
        # Frontera izquierda
        if self.condiciones_frontera['izquierda'] == 'Dirichlet':
            self.u[0, :] = 0.0
        elif self.condiciones_frontera['izquierda'] == 'Neumann':
            self.u[0, :] = self.u[1, :]
        elif self.condiciones_frontera['izquierda'] == 'Periódica':
            self.u[0, :] = self.u[-2, :]  # Igual que el borde derecho
        # Frontera derecha
        if self.condiciones_frontera['derecha'] == 'Dirichlet':
            self.u[-1, :] = 0.0
        elif self.condiciones_frontera['derecha'] == 'Neumann':
            self.u[-1, :] = self.u[-2, :]
        elif self.condiciones_frontera['derecha'] == 'Periódica':
            self.u[-1, :] = self.u[1, :]  #Igual que el borde izquierdo
        # Frontera inferior
        if self.condiciones_frontera['inferior'] == 'Dirichlet':
            self.u[:, 0] = 0.0
        elif self.condiciones_frontera['inferior'] == 'Neumann':
            self.u[:, 0] = self.u[:, 1]
        elif self.condiciones_frontera['inferior'] == 'Periódica':
            self.u[:, 0] = self.u[:, -2]  # Igual que el borde superior
        # Frontera superior
        if self.condiciones_frontera['superior'] == 'Dirichlet':
            self.u[:, -1] = 0.0
        elif self.condiciones_frontera['superior'] == 'Neumann':
            self.u[:, -1] = self.u[:, -2]
        elif self.condiciones_frontera['superior'] == 'Periódica':
            self.u[:, -1] = self.u[:, 1]  # Igual que el borde inferior
    
    def resolucion_ec_calor(self):
        """
        Ejecuta la simulación de la ecuación de calor en 2D.
        """
        self.definir_condiciones_iniciales()
        self.definir_condiciones_de_frontera()

        for n in range(self.Nt):
            # Actualizar la malla de temperatura
            self.u_new[1:-1, 1:-1] = (
                self.u[1:-1, 1:-1]
                + self.alpha_x * (self.u[2:, 1:-1] - 2 * self.u[1:-1, 1:-1] + self.u[:-2, 1:-1])
                + self.alpha_y * (self.u[1:-1, 2:] - 2 * self.u[1:-1, 1:-1] + self.u[1:-1, :-2])
            )

            self.definir_condiciones_de_frontera()
            self.u[:, :] = self.u_new[:, :]

            # Guardar estados para la animación
            if n % (self.Nt // 100) == 0:  # Guardar cada 1% del progreso
                self.snapshots.append(np.copy(self.u))

    def animar(self):
        """
        Crea una animación de la evolución de la temperatura.
        """
        fig, ax = plt.subplots(figsize=(6, 6))
        img = ax.imshow(self.snapshots[0], extent=[0, self.Lx, 0, self.Ly], origin='lower', cmap='hot')
        cbar = plt.colorbar(img, ax=ax)
        cbar.set_label('Temperatura (k) ')

        def actualizar(frame):
            img.set_data(self.snapshots[frame])
            ax.set_title(f'Tiempo: {frame * self.dt:.2f} s')

        ani = FuncAnimation(fig, actualizar, frames=len(self.snapshots), interval=50)
        plt.show()


# Menú de configuración
def menu():
    print("\n--- Menú Principal ---")
    print("1. Configurar parámetros del dominio")
    print("2. Configurar condiciones de frontera")
    print("3. Configurar condición inicial")
    print("4. Resolver y animar ecuación del calor")
    print("5. Salir")
    return input("Seleccione una opción: ")

def configurar_dominio():
    Lx = float(input("Largo del dominio en x (metros): "))
    Ly = float(input("Largo del dominio en y (metros): "))
    Nx = int(input("Número de puntos de la malla en x: "))
    Ny = int(input("Número de puntos de la malla en y: "))
    dt = float(input("Paso de tiempo (segundos): "))
    c = float(input("Constante de difusión del calor: "))
    T = float(input("Tiempo máximo de simulación (segundos): "))
    tmax = float(input("Temperatura máxima inicial: "))
    return {"Lx": Lx, "Ly": Ly, "Nx": Nx, "Ny": Ny, "dt": dt, "c": c, "T": T, "tmax": tmax}

def configurar_condiciones_frontera():
    opciones = ["Dirichlet", "Neumann", "Periódica"]
    frontera = {}
    for lado in ["izquierda", "derecha", "inferior", "superior"]:
        print(f"Condiciones disponibles para {lado}: {', '.join(opciones)}")
        frontera[lado] = input(f"Condición para la frontera {lado}: ")
    return frontera

def configurar_condicion_inicial():
    opciones = ["punto_caliente", "gradiente_lineal", "onda_sinusoidal", "uniforme"]
    print(f"Opciones: {', '.join(opciones)}")
    return input("Seleccione una condición inicial: ")

# Control del programa
def main():
    parametros = None
    condiciones_frontera = None
    condicion_inicial = None

    while True:
        opcion = menu()
        if opcion == "1":
            parametros = configurar_dominio()
        elif opcion == "2":
            condiciones_frontera = configurar_condiciones_frontera()
        elif opcion == "3":
            condicion_inicial = configurar_condicion_inicial()
        elif opcion == "4":
            if parametros and condiciones_frontera and condicion_inicial:
                ecuacion = ecuacion_calor(
                    **parametros,
                    condiciones_frontera=condiciones_frontera,
                    condiciones_iniciales=condicion_inicial
                )
                ecuacion.resolucion_ec_calor()
                ecuacion.animar()
            else:
                print("Faltan configuraciones. Complete las configuraciones antes de resolver.")
        elif opcion == "5":
            print("¡Saliendo!")
            break
        else:
            print("Opción no válida, intente de nuevo.")


if __name__ == "__main__":
    main()

