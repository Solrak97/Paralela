from PIL import Image
import numpy as np
from moviepy.editor import ImageSequenceClip
from mpi4py import MPI
import sys, getopt
import random
import copy




#Obtiene los parametros por linea de comandos
def parametros(argv):
	n = int(sys.argv[1])
	i = int(sys.argv[2])
	return (n, i)




#Funcion main
def main(argv):
	comm = MPI.COMM_WORLD
	size = comm.size
	pid = comm.rank

	#Parametros
	tamano = 0
	iterador = 0
	segmentos = []
	lista_estados = []

	if pid == 0:
		tamano, iterador = parametros(argv)
		matrix = np.full((tamano, tamano), False, dtype = bool)
		rellenaMatriz(matrix, 70)
		lista_estados.append(matrix)

	#bcast de parametros iniciales
	tamano = comm.bcast(tamano, root = 0)
	iterador = comm.bcast(iterador, root = 0)


	for indice in range(iterador):

		#Division de los datos para el scatter en grupos de columnas vecinas
		if pid == 0:
			segmentos = divisor_triadas(matrix, tamano)
			segmentos = agrupador(segmentos, tamano, size)
		
		#Scatter de elementos
		segmentos = comm.scatter(segmentos, root = 0)

		#Aplica la regla a elementos
		segmento_actualizado = []
		for seccion in segmentos:
			for i in range(tamano):
				seccion_copy = copy.deepcopy(seccion)
				aplicar_regla((1, i), tamano, seccion, seccion_copy)
		

			segmento_actualizado.append(seccion[1])

		#Recoleccion de segmentos
		matrix = comm.gather(segmento_actualizado, root = 0)
		
		if pid == 0:
			matrix = retypear(matrix, tamano)
			lista_estados.append(matrix)

	#Generacion del gif
	if pid == 0:
		generador(lista_estados, tamano, tamano)



#Cuenta la cantida de vecinos que tiene la coordenada
def calcula_vecinos (coordenada, dimension, matriz):
	actual_x, actual_y = coordenada
	cant_vecinos = 0
	vecinos = [(1, 0), (1 ,1), (0 ,1), (-1 ,1), (-1 ,0), (-1 ,-1), (0 ,-1), (1 ,-1)]
	
	for coordenada in vecinos:
		x, y = coordenada
		cant_vecinos += matriz [(actual_x + x) % dimension][(actual_y + y) % dimension]

	return cant_vecinos





#Funcion que aplicas reglas para la celda
def aplicar_regla(coordenada, dimension, seccion, seccion_copy):
	vecinos = calcula_vecinos(coordenada, dimension, seccion)

	x, y = coordenada

	#0,1,7 mueren
	if vecinos == 0 or vecinos == 1 or vecinos == 7:
		seccion[x][y] = False

	#3,6,8 crea vida
	elif (vecinos == 3  or vecinos == 6 or vecinos == 8):
		seccion[x][y] = True


		#2,4,5 mantiene vida
	pass




#Divisor de matriz en grupos de columansa vecinas
def divisor_triadas(matriz, tamano):
	lista_triadas = []
	for i in range(0, tamano):
		lista_triadas.append([matriz[(i - 1)%tamano], matriz[i], matriz[(i + 1)%tamano]])
	return lista_triadas



#Agrupador de las triadas en cantidad de procesos
def agrupador(lista, tamano, procesos):
	div = tamano // procesos
	grupos = []
	for i in range (0, len(lista), div):
		subgrupos = []
		for j in range(0, div):
			subgrupos.append(lista[i + j])
		grupos.append(subgrupos)
	return grupos


#Rellena la matriz de forma aleatoria
def rellenaMatriz(matriz, treshold):
	for i in range(len(matriz)):
		for j in range(len(matriz)):
			matriz[i][j] = (treshold < random.uniform(0, 100))
	pass




#Generador de gifs del profesor
def generador(lista_estados, cf, cc):
	imagenes = []
	for md in lista_estados:
		mi = np.zeros([cf, cc, 3], dtype=np.uint8)
		for i in range(cf):
			for j in range(cc):
				mi[i,j] = [255, 128, 0] if (md[i,j] == True) else [0, 0, 255]
		imagenes.append(mi)
	clip = ImageSequenceClip(imagenes,fps=1) # fps = 1 indica un cuadro por segundo
	clip.write_gif('automata.gif')
	clip.close()
	return





#Por alguna razon al hacer el gather se pierde el tipo de dato
#Así que esto lo vuelve a poner en el tipo de dato correcto
#Thats Illegal pero si... lol
def retypear(matrix, tamano):
	secciones = []
	
	matriz = np.full((tamano, tamano), False, dtype = bool)

	for seccion in matrix:
		for subconjunto in seccion:
			for elemento in subconjunto:
				secciones.append(elemento)

	cont = 0
	for i in range(tamano):
		for j in range(tamano):
			matriz[i][j] = secciones[cont]
			cont += 1

	return matriz




#Llamado al main
main(sys.argv)