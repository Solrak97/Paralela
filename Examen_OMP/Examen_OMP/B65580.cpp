#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <omp.h> 
#include <iostream>
#include <vector>
using namespace std;

typedef vector< vector< int > > T_campo;	// representa el estado del campo
typedef vector< vector< double > > T_campo_riesgo;	// representa el riesgo de cada casilla oculta
int CH = 0;

// REQ: (0 < p < 1)
// MOD: campo
// EFE: genera al azar, con probabilidad p, el campo de minas representándolas con un 10.
// RET: campo por referencia con las minas asignadas.
//		la cantidad de minas generadas.
// NOTA: p representa la probabilidad de que haya una mina en una casilla del campo
int genera_minas(double p, T_campo& campo);

// MOD: campo
// EFE: llena cada posición del campo donde no hay mina con la cantidad de minas adyacentes.
// RET: campo por referencia con el conteo de minas adyacentes a cada casilla sin mina.
// NOTA: El campo es un toroide, por tanto todas las casillas del campo tienen ocho adyacentes.
void cuenta_minas(T_campo& campo);

/* NO TIENE QUE PROGRAMARLA */
// REQ: campo_visible[f][c] == -1.
// EFE: determina el riesgo de encontrar una mina en campo_visible[f][c].
// RET: un valor entre -1 y 0 que mide el riesgo de la casilla.
double estima_riesgo_casilla(const T_campo& campo_visible, int f, int c);

// REQ: para todo (0 < f,c < N): -1 <= campo_visible[f][c] <= 8
// NOTA: si campo_visible[f][c] == -1 es porque no se tiene un mejor estimado del riesgo.
// MOD: campo_riesgo
// EFE: estima el riesgo de cada posición en -1 con un número r tal que -1 < r < 0.
// NOTA: se basa en estima_riesgo_casilla(const T_campo& campo_visible, int f, int c).
void estima_riesgo_campo(const T_campo& campo_visible, T_campo_riesgo& campo_riesgo);

int main(int argc, char* argv[]) {
	int N = 0; // cantidad de filas y columnas
	double p = 0.0; // probabilidad de existencia de mina
	int ch = 0; // cantidad de hilos
		
	while ((N <= 0) && ((p <= 0.0) || (p >= 1.0)) && (ch < 2)){
		cout << "N: "; cin >> N;
		cout << "p: "; cin >> p;
		cout << "ch: "; cin >> ch;
	} // suponga que N % ch == 0, N es divisible por ch


	CH = ch;

	/* Se crean  las matrices que representarán al campo minado: */
	vector< int > fila_campo_real;
	fila_campo_real.resize(N, 0);		// N elementos inicializados en cero
	T_campo campo_real;	// representa el estado completo del campo
	campo_real.resize(N, fila_campo_real);	// N filas inicializadas con fila_campo_real
	
	vector< int > fila_campo_visible;
	fila_campo_visible.resize(N, -1);		// N elementos inicializados en -1 indicando que no son visibles
	T_campo campo_visible; // representa el estado visible del campo
	campo_visible.resize(N, fila_campo_visible); // N filas inicializadas con fila_campo_visible
	
	vector< double > fila_campo_riesgo;
	fila_campo_riesgo.resize(N, -1.0);		// N elementos inicializados en -1 indicando que no son visibles
	T_campo_riesgo campo_riesgo; // representa el estado visible del campo
	campo_riesgo.resize(N, fila_campo_riesgo); // N filas inicializadas con fila_campo_visible
	srand (time(NULL));
	
	// se inicializa el campo_real:
	int cm = genera_minas(p,campo_real);
	
	// cuenta minas:
	cuenta_minas(campo_real);

	// se estima el riesgo del campo visible:
	estima_riesgo_campo(campo_visible, campo_riesgo);

	return 0;
}  /* main */


int genera_minas(double p, T_campo& campo) {
	return 0;
}

void cuenta_minas(T_campo& campo) {
	int minas = 0;
	for (int f = 0; f < campo.size(); f++) {
		for (int c = 0; c < campo.size(); c++) {
			if (campo[c - 1 % campo.size()][f - 1 % campo.size()] == 10) {
				#pragma omp atomic
				minas++;
			}
			if (campo[c][f + 1 % campo.size()] == 10) {
				#pragma omp atomic
				minas++;
			}
			if (campo[c + 1 % campo.size()][f + 1 % campo.size()] == 10) {
				#pragma omp atomic
				minas++;
			}
			if (campo[c - 1 % campo.size()][f] == 10) {
				#pragma omp atomic
				minas++;
			}
			if (campo[c][f + 1 % campo.size()] == 10) {
				#pragma omp atomic
				minas++;
			}
			if (campo[c - 1 % campo.size()][f - 1 % campo.size()] == 10) {
				#pragma omp atomic
				minas++;
			}
			if (campo[c][f - 1 % campo.size()] == 10) {
				#pragma omp atomic
				minas++;
			}
			if (campo[c - 1 % campo.size()][f - 1 % campo.size()] == 10) {
				#pragma omp atomic
				minas++;
			}
		}
	}
}

double estima_riesgo_casilla(const T_campo& campo_visible, int f, int c) {
	return -(rand()%10000/10000.0); // NO LO MODIFIQUE
}

void estima_riesgo_campo(const T_campo& campo_visible, T_campo_riesgo& campo_riesgo) {
	//Ya que las posiciones de memoria (f,c) son independientes entre ellas no es necesario
	//Usar bgloqueos de memoria

	//Se paraleliza el for, con un for anidado (Para eso el collapse de 2 niveles)
	//Esto recorrera cada casilla del campo de riesgo
	#pragma omp parallel for collapse(2) num_threads(CH)
	for (int f = 0; f < campo_riesgo.size(); f++) {
		for (int c = 0; c < campo_riesgo.size(); c++) {
			//Si el campo de riesgo no tiene una mejor evaluacion que en una iteracion anteriro, se evalua de nuevo
			if(campo_visible[f][c] == -1){
				campo_riesgo[f][c] = estima_riesgo_casilla(campo_visible, f, c);
			}
		}
	}
}