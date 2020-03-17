
import numpy as np
import random

class GrafoPesado:
    """Representa una red compleja de vertices (vrt) y adyacencias (ady).
    SSPUESTOS:
    1. los vertices se identifican con enteros entre 0 y n
    2. las aristas tienen pesos mayores o iguales a cero. """

    #EFE: Construye un GrafoPesado vacio de nxn.
    def __init__(self, n = 0):
        self.n = n                                  #Se guarda la cantidad de vertices
        self.matriz_adys = np.empty( (n, n) )       #Representa la matriz de adyacencias
        self.matriz_adys[:] = float('inf')          #Se inicialzan las aristas en float('inf')
        return

    #REQ: 0 < p < 1
    #EFE: Inicializa un GrafoPesado previamente creado con n vertices en el que el
    #conjunto de adyacencias se determina aleatoriamente utilizando p como
    #la probabilidad de que exista una adyacencia entre cualesquiera
    #dos vertices. El peso de cada arista se escoge al azar uniformemente
    #entre 0 y max - 1.
    def init__random(self, p: float, max: int):
        for i in range(self.n):
            for j in range(i + 1, self.n):
                if(random.random() < p): #Se genera un valor aleatorio entre 0 y 1.
                    peso = random.randint(0, max - 1)
                    self.matriz_adys[i][j] = peso
                    self.matriz_adys[j][i] = peso
        return

    #EFE: Retorna true si 0 <= idvrt < n
    #NOTA: idvrt significa "identificador de vertice".
    def existe_vrt(self, idvrt: int):
        return 0 <= idvrt and idvrt < n

    #EFE: retorna true si existe adyacencia entre los vertices idvrt_o e idvrt_d
    def existe_ady(self, idvrt_o: int, idvrt_d: int):
        return existe_vrt(idvrt_o) and existe_vrt(idvrt_d) and self.matriz_adys([idvrt_o][idvrt_d] != float('inf'))

    #REQ: 0 <= idvrt < n.
    #EFE: retorna los idvrts de todos los verticesadyacentes a idVrt.
    def obt_idvrt_adys(self, idvrt: int):
        rsl = []
        for i in range(self.n):
            if (self.matriz_adys[idvrt][i] != float('inf')):
                rsl.append(i)
        return rsl

    #REQ: 0 <= idvrt < n.
    #EFE: retorna el peso de la arista (idvrt_o, idvert_d).
    #NOTA: implementa el operador [a,b,...]
    def __getitem__(self, indices):
        idvrt_o, idvrt_d = indices
        return self.matriz_adys[idvrt_o][idvrt_d]

    #EFE: retorna el total de vertices en el GrafoPesado
    def obt_total_vrts(self):
        return self.n

    #EFE: retorna el total de adyacencias en el GrafoPesado
    def obt_total_adys(self):
        sum_adys = 0
        for i in range(self.n):
            for j in range(i + 1, self.n):
                if(self.matriz_adys[i][j] != float('inf')):
                    sum_adys += 1
        return sum_adys

    def ver_matriz(self):
        return np.array2string(self.matriz_adys)
