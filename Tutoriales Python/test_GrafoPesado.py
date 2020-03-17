import random
from grafo_pesado import GrafoPesado

def main():
    #genera semilla para numeros aleatorios,
    #OJO: no se puede invocar enel constructor porque repite los valores,
    random.seed()

    #construye un grafo con n == 10
    mi_grafo = GrafoPesado(10)

    #inicializa al azar el grafo previamente construido con n == 10
    mi_grafo.init__random(0.5, 10)

    #Se verifica la matriz de adyacencia interna
    print(mi_grafo.ver_matriz())

    #se despliegan los vertices adyacentes a cada vertice (0 <= peso) and (peso < float('inf'))
    for i in range(10):
        print(mi_grafo.obt_idvrt_adys(i))

    #se obtienen los pesos de cada arista y se imprimen siempre que existen
    for i in range(10):
        for j in range(10):
            if (mi_grafo[i,j] < float('inf')):
                print(mi_grafo[i,j])
    print("Total de vertices: ", mi_grafo.obt_total_vrts())
    print("Total de adyacencias: ", mi_grafo.obt_total_adys())

main() #invoca la funcion main
