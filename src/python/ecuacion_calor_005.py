import numpy as np
import matplotlib.pyplot as plt

class ecuacion_calor:
    def __init__(self, Lx, Ly, Nx, Ny, dt, c, condiciones_frontera, condiciones_iniciales, tmax):
        """
        Inicializa los parámetros de la simulación.
        
        :param Lx: Largo del dominio en x (metros)
        :param Ly: Largo del dominio en y (metros)
        :param Nx: Número de puntos de la malla en x
        :param Ny: Número de puntos de la malla en y
        :param T: Tiempo total de simulación (segundos)
        :param dt: Paso de tiempo (segundos)
        :param c: Velocidad de propagación del calor (constante de difusión)
        :param boundary_conditions: Diccionario con tipos de condiciones de frontera para cada borde
        :param initial_condition: Tipo de condición inicial para la temperatura
        """
        self.tmax = tmax
        self.Lx = Lx
        self.Ly = Ly
        self.Nx = Nx
        self.Ny = Ny
        #self.T = T
        self.dt = dt
        self.Nt = int(T / dt)
        self.c = c
        self.condiciones_frontera = condiciones_frontera
        self.condiciones_iniciales = condiciones_iniciales
        
        # Crear la malla en x y y usando linspace
        self.x = np.linspace(0, Lx, Nx)
        self.y = np.linspace(0, Ly, Ny)
        self.dx = self.x[1] - self.x[0]
        self.dy = self.y[1] - self.y[0]
        
        # Parámetros de estabilidad de Courant para cada dirección
        self.alpha_x = c**2 * dt / self.dx**2
        self.alpha_y = c**2 * dt / self.dy**2
        if self.alpha_x >= 0.5 or self.alpha_y >= 0.5:
            raise ValueError("Condición de estabilidad violada: reduce dt o incrementa Nx/Ny.")
        
        # Inicialización de la malla de temperatura
        self.u = np.zeros((Nx, Ny))  # Temperatura en el tiempo actual
        self.u_new = np.zeros((Nx, Ny))  # Temperatura en el tiempo siguiente

    def definir_condiciones_iniciales(self):
        """
        Establece la condición inicial de temperatura en el dominio según el tipo especificado.
        """
        if self.condiciones_iniciales == 'punto_caliente':
            for i in range(self.Nx):
                for j in range(self.Ny):
                    if 0.4 * self.Lx <= self.x[i] <= 0.6 * self.Lx and 0.4 * self.Ly <= self.y[j] <= 0.6 * self.Ly:
                        self.u[i, j] = self.tmax # Región caliente en el centro
                        
        elif self.condiciones_iniciales == 'gradiente_lineal':
            for i in range(self.Nx):
                self.u[i, :] = self.tmax* (self.x[i] / self.Lx)  # Gradiente lineal en x
            
        elif self.condiciones_iniciales == 'onda_sinusoidal':
            for i in range(self.Nx):
                for j in range(self.Ny):
                    self.u[i, j] = self.tmax * np.sin(np.pi * self.x[i] / self.Lx) * np.sin(np.pi * self.y[j] / self.Ly)
                    
        elif self.condiciones_iniciales == 'uniforme':
            self.u[:, :] = self.tmax  # Temperatura uniforme en todo el dominio

    def definir_condiciones_de_frontera(self):
        """
        Aplica las condiciones de frontera según el tipo especificado.
        """
        # Frontera izquierda
        if self.condiciones_frontera['izquierda'] == 'Dirichlet':
            self.u[0, :] = 0.0
        elif self.condiciones_frontera['izquierda'] == 'Neumann':
            self.u[0, :] = self.u[1, :]  # Gradiente cero
        elif self.condiciones_frontera['izquierda'] == 'Periódica':
            self.u[0, :] = self.u[-2, :]  # Conecta con el borde derecho
        
        # Frontera derecha
        if self.condiciones_frontera['derecha'] == 'Dirichlet':
            self.u[-1, :] = 0.0
        elif self.condiciones_frontera['derecha'] == 'Neumann':
            self.u[-1, :] = self.u[-2, :]  # Gradiente cero
        elif self.condiciones_frontera['derecha'] == 'Periódica':
            self.u[-1, :] = self.u[1, :]  # Conecta con el borde izquierdo
        
        # Frontera inferior
        if self.condiciones_frontera['inferior'] == 'Dirichlet':
            self.u[:, 0] = 0.0
        elif self.condiciones_frontera['inferior'] == 'Neumann':
            self.u[:, 0] = self.u[:, 1]  # Gradiente cero
        elif self.condiciones_frontera['inferior'] == 'Periódica':
            self.u[:, 0] = self.u[:, -2]  # Conecta con el borde superior
        
        # Frontera superior
        if self.condiciones_frontera['superior'] == 'Dirichlet':
            self.u[:, -1] = 0.0
        elif self.condiciones_frontera['superior'] == 'Neumann':
            self.u[:, -1] = self.u[:, -2]  # Gradiente cero
        elif self.condiciones_frontera['superior'] == 'Periódica':
            self.u[:, -1] = self.u[:, 1]  # Conecta con el borde inferior

    def resolucion_ec_calor(self):
        """
        Ejecuta la simulación de la ecuación de calor en 2D.
        """
        # Establecer condiciones iniciales y de frontera
        self.definir_condiciones_iniciales()
        self.definir_condiciones_de_frontera()

        # Simulación
        for n in range(self.Nt):
            # Calcular u_new en el tiempo siguiente usando el esquema de diferencias finitas
            for i in range(1, self.Nx - 1):
                for j in range(1, self.Ny - 1):
                    self.u_new[i, j] = self.u[i, j] + self.alpha_x * (
                        self.u[i+1, j] - 2*self.u[i, j] + self.u[i-1, j]
                    ) + self.alpha_y * (
                        self.u[i, j+1] - 2*self.u[i, j] + self.u[i, j-1]
                    )
            
            # Actualizar las condiciones de frontera
            self.definir_condiciones_de_frontera()
            
            # Actualizar u para el siguiente paso de tiempo
            self.u[:, :] = self.u_new[:, :]

            # Opcional: Mostrar el progreso cada cierto número de pasos de tiempo
            if n % (self.Nt // 10) == 0:
                self.plot(n * self.dt)

        # Mostrar el resultado final
        self.plot(self.T)

    def plot(self, current_time):
        """
        Grafica la distribución de temperatura en el dominio.
        
        :param current_time: Tiempo actual de la simulación
        """
        plt.imshow(self.u, extent=[0, self.Lx, 0, self.Ly], origin='lower', cmap='hot')
        plt.colorbar(label='Temperatura')
        plt.title(f'Tiempo: {current_time:.4f} s')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.pause(0.1)

# Uso de la clase HeatEquationSolver
# Aquí puedes definir los valores de Lx y Ly, tipos de condiciones de frontera y condición inicial

""""IMPORTANTE" ##main solicitando datos
int main(Lx, Ly, Nx, Ny)
c = input ("Ingrese el valor de la constante de difusión:")# Constante de difusión del calor, LA DEJAMOS AS+Í O PEDIMOS LOS DATOS PARA CALCULARLA?
Lx = input ("Ingrese el valor del dominio en x:")# largo del dominio en x (Lx) en metros
Ly = input ("Ingrese el valor del dominio en y:") largo del dominio en y (Ly) en metros
Nx = input ("Ingrese la cantidad de puntos en x:")# Número de puntos de la malla en x CREO QUE ESTOS SI NOS TOCA PONERLOS A NOSOTROS!!
Ny = input ("Ingrese la cantidad de puntos en y:")# Número de puntos de la malla en y
dt = input ("Ingrese los segundos calculados*:")# Paso de tiempo en segundos
"""
def menu():
# que el menu pida condiciones de frontera y las condiciones iniciales para luego definir las variables de esa manera ok?
condiciones_frontera = 0 #establezco las condiciones de frontera
    'izquierda': '',   # Opciones: 'Dirichlet', 'Neumann', 'Periódica'
    'derecha': '',
    'inferior': '',
    'superior': '',
    print("Escoga la condición para cada extremo:")
    print("D: Dirichlet")
    print("N: Neumann")
    print("P: Periódica")

opcion = ""
while opcion != "D", "N", "P":
  opcion = input("Ingrese una opción: ")
  if opcion == "D" or "N" or "P":
    condiciones_frontera['izquierda'] = input("Lado izquierdo: ")
    condiciones_frontera['derecha'] = input("Lado derecho: ")
    condiciones_frontera['inferior'] = input("Lado inferior: ")
    condiciones_frontera['superior'] = input("Lado superior: ")
    
	for lado, condicion in condiciones_frontera:
        print(f"{lado.capitalize()}: {condicion}")

    return condiciones_frontera
}
# Configura la condición inicial
condiciones_frontera = 'punto_caliente'  # Opciones: 'punto_caliente', 'gradiente_lineal', 'onda ##hablarlo con el grupo como desarrollar




### ya para generarlo
ecuacion = ecuacion_calor("Lx, Ly, Nx, Ny, dt, c, condiciones_frontera, condiciones_iniciales, tmax")
ecuacion.resolucion_ec_()
