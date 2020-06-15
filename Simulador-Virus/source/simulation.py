from mpi4py import 	MPI
import sys, getopt
import numpy as np
import math
from models import Individual, Bundled_Metadata
from aux_functions import dict2list, chunkIt, appendModel
import random
import collections
from graph import graph

def main(argv):
	
	#MPI COMM PROTOCOL

	comm = MPI.COMM_WORLD
	size = comm.size
	pid = comm.rank


	#PROCESS SHARED VARIABLES
	tmm = 0
	params = []
	modeled_data = []

	total_infected = 0
	total_deceased = 0
	total_immune = 0

	log_data = []
	


	if pid == 0:
		
		#PARAM EXTRACTION
		params = param_extractor(argv)
		cpr, piv, prj, prm, poi, toc, dmn, dmx, rmj, rmm, vmj, vmm, drc = params

		#SETUP		

		#Side size calc of the square matrix based on number of people and crowding of spaces
		#tmm = math.floor(math.sqrt(cpr/toc))+1.
		tmm = math.floor(math.sqrt(cpr/toc)) + 1
		
		#initial population and infection of the model		
		#maybe I could do the infection during  the population, but not sure on how
		#to do it  yet
		modeled_data = create_model(cpr, tmm, poi, dmn, dmx)
		modeled_data = dict2list(modeled_data)



		#SETUP BROADCASTING

	tmm = comm.bcast(tmm, root = 0)

	cpr, piv, prj, prm, poi, toc, dmn, dmx, rmj, rmm, vmj, vmm, drc = comm.bcast(params, root = 0)
	cpr = int(cpr)
	poi = int(poi)
	dmn = int(dmn)
	dmx = int(dmx)
	rmj = int(rmj)
	rmm = int(rmm)
	vmj = int(vmj)
	vmm = int(vmm)
	drc = int(drc)


		
		#TIC SIMULATIONS

	comm.Barrier()
	time = MPI.Wtime()
						#drc
	for i in range(1, drc + 1):
			#TICS
		#MODEL GATHERING
		#maybe a different method is possible?
		if pid == 0:
			modeled_data = chunkIt(modeled_data, size)
		modeled_data = comm.scatter(modeled_data, root = 0)

		#TIC SIMULATION
		susceptible, infected, deceased, immune = simulate_tic(prj, prm, piv, dmn, dmx, rmj, rmm, vmj, vmm, tmm, modeled_data)


		#DATA REDUCE
		partial_susceptible = comm.reduce(susceptible, op=MPI.SUM)
		partial_infected = comm.reduce(infected, op=MPI.SUM)
		partial_deceased = comm.reduce(deceased, op=MPI.SUM)
		partial_immune = comm.reduce(immune, op=MPI.SUM)
		
		

		#RE-SORTING
		modeled_data = comm.gather(modeled_data, root = 0)
		if pid == 0:
			model_map = collections.defaultdict(list)
			for super_list in modeled_data:
				for data_list in super_list:
					for element in data_list:
						appendModel(element, model_map)

			modeled_data = dict2list(model_map)



		#DATA WORK
		if pid == 0:
			susceptible = (partial_susceptible / i, partial_susceptible * 100 / cpr, partial_susceptible)
			infected = (partial_infected / i, partial_infected * 100 / cpr, partial_infected)
			deceased = (partial_deceased / i, partial_deceased * 100 / cpr, partial_deceased)
			immune = (partial_immune / i, partial_immune * 100 / cpr, partial_immune)
			total_infected += partial_infected - total_infected
			total_deceased += partial_deceased - total_deceased
			total_immune += partial_immune - total_immune
			log_data.append(Bundled_Metadata(susceptible, infected, deceased, immune))
			
	

	#CALCULATE EXECUTION TIME
	final_time = MPI.Wtime() - time
	final_time = comm.reduce(final_time, op=MPI.MAX)
	with open("Time.txt", "w") as file:
		file.write("FINAL TIME: " + str(final_time))


		#LOGGING AND PLOTTING
	if pid == 0:
		with open("Log.txt", "w") as file:
			data = "============================================================================================ \n"
			for element in log_data:
				data += "Susceptible:" + "\n"
				data += "Promedio por tic: " + str(element.susceptible[0]) + " Porcentaje: " + str(element.susceptible[1]) + " Cantidad actual: " + str(element.susceptible[2]) + "\n"
				data += "Infected:" + "\n"
				data += "Promedio por tic: " + str(element.infected[0]) + " Porcentaje: " + str(element.infected[1]) + " Cantidad actual: "+ str(element.infected[2]) +  "\n"
				data += "Deceased:" + "\n"
				data += "Promedio por tic: " + str(element.dead[0]) + " Porcentaje: " + str(element.dead[1]) + " Cantidad actual: " + str(element.dead[2]) + "\n"
				data += "Immune:" + "\n"
				data += "Promedio por tic: " + str(element.immune[0]) + " Porcentaje: " + str(element.immune[1]) + " Cantidad actual: " + str(element.immune[2]) + "\n"
				data += "============================================================================================ \n\n"	
			
			total_succeptible = cpr - (total_immune + total_deceased + total_infected)

			data += "Resultados totales:" + "\n"
			data += "Susceptible:" + "\n"
			data += "Promedio total: " + str(total_succeptible / drc) + " Porcentaje: " + str(total_succeptible * 100 / cpr) + " Cantidad: " + str(total_succeptible) + "\n"
			data += "Infected:" + "\n"
			data += "Promedio por tic: " + str(total_infected / drc) + " Porcentaje: " + str(total_infected * 100 / cpr) + " Cantidad: "+ str(total_infected) +  "\n"
			data += "Deceased:" + "\n"
			data += "Promedio por tic: " + str(total_deceased / drc) + " Porcentaje: " + str(total_deceased * 100 / cpr) + " Cantidad: " + str(total_deceased) + "\n"
			data += "Immune:" + "\n"
			data += "Promedio por tic: " + str(total_immune / cpr) + " Porcentaje: " + str(total_immune * 100 / cpr) + " Cantidad: " + str(total_immune) + "\n"
			data += "============================================================================================ \n\n"	
			file.write(data) 
	
		plotter_data = []
		for element in log_data:
			plotter_data.append((element.susceptible[1], element.infected[1], element.dead[1], element.immune[1]))
		graph(plotter_data)
	pass






#Params CPR, TMM, POI, DMN, DMX
#returns dictionary of people with poi people infected
#poi and age ared distributed the same
#maybe there is a way of doing it on just one loop?
#the return format is {(x,y):[individual_list]}
def create_model(cpr,tmm, poi, dmn, dmx):
	model_map = collections.defaultdict(list)
	infected = poi

#Creates young people
	for element in range(round(int(cpr) * 0.9045)):
		x = random.randint(0, tmm - 1)
		y = random.randint(0, tmm - 1)
		element = Individual((x, y), True)
		if infected > poi - (poi * 0.9045):
			element.infect(dmn, dmx)
			infected -= 1

		key = element.get_destination()
		if key in model_map:
			model_map[key].append(element)
		else:
			model_map[key] = [element]
		
#Creates old people
	for element in range(round(int(cpr) * 0.0955)):
		x = random.randint(0, tmm - 1)
		y = random.randint(0, tmm - 1)
		element = Individual((x, y), False)
		if infected > 0:
			element.infect(dmn, dmx)
			infected -= 1
		key = element.get_destination()
		if key in model_map:
			model_map[key].append(element)
		else:
			model_map[key] = [element]
	
	return model_map





#Simulates an illnes tic and gets the data from a single process
def simulate_tic(prj, prm, piv, dmn, dmx, rmj, rmm, vmj, vmm, tmm, model):
	susceptible = 0
	deceased = 0
	immune = 0
	infected = 0
	index = 0
	for data_list in model:
		local_infected = 0
		for element in data_list:
			if element.illness_countdown != -1:
				local_infected += 1
		for element in data_list:
			result = element.calculate_illness(prj, prm, local_infected, piv, dmn, dmx)
			
			#The movement is calculated here
			element.calculate_movement(index, tmm, rmj, rmm, vmj, vmm)
			
			#This part decodes if the individual was infected, died or got immunized
			if result == -1:
				susceptible += 1
			if result == 0:
				deceased += 1
				del element
			if result == 1:
				immune += 1
			if result == 2:
				infected += 1
	return (susceptible, infected, deceased, immune)



#I want to get rid of that for loop, is just awkward
#But hey... if it works
def param_extractor(argv):
    try:
    	opts, args = getopt.getopt(argv,"f:")
    except getopt.GetoptError:
        print ('Simulation -f <path to parameter file> \n file has to be a csv ')
        sys.exit(2)
    for opt, arg in opts:
    	if opt in ("-f"):
            return np.genfromtxt(arg, delimiter=',')
            	






if __name__ == '__main__':
	main(sys.argv[1:])