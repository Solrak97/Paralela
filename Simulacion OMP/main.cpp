#include "Utility.h"
#include <math.h>

#define PARAMS_C 13

int main(int argc, char * argv[]) 
{
	//Param "error handler"
	if (argc - 1 != PARAMS_C) {
		std::cout << "La cantidad de parametros no es correcta\n" << 
		"asegurese de utilizar el siguiente formato\n" <<
		"cpr piv prj prm poi toc dmn dmx rmj rmm vmj vmm drc" << std::endl;
		exit(0);
	}

	//			PARAMS
	int cpr = atoi(argv[1]);
	double piv = atof(argv[2]);
	double prj = atof(argv[3]);
	double prm = atof(argv[4]);
	int poi = atoi(argv[5]);
	double toc = atof(argv[6]);
	int dmn = atoi(argv[7]);
	int dmx = atoi(argv[8]);
	int rmj = atoi(argv[9]);
	int rmm = atoi(argv[10]);
	int vmj = atoi(argv[11]);
	int vmm = atoi(argv[12]);
	int drc = atoi(argv[13]);
	 
	//MAX THREAD FIXED FROM CORES AND USER INPUT
	int MAX_THREADS = Utility::get_max_threads();
	
	//TMM Calc
	int tmm = floor(sqrt(cpr / toc)) + 1;
	
	//Matrix definition
	std::vector<std::vector<std::list<int>>> matrix;
	matrix.resize(tmm, std::vector<std::list<int>>(tmm));
	std::cout << "Size: " << matrix.size() << std::endl;
 
}