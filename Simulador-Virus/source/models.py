import random
from aux_functions import vecMul, scalarMul, sumVec, vecAbs

#Class Individual, represents an individual on the simulation model
class Individual():
	
	#home is the first assigned direction
	def __init__(self, home, underage):
		self.home = home
		self.underage = underage
		self.alive = True

	#days of sickness and immunity will be countdowns
		self.immunity_countdown = 0
		self.illness_countdown = -1

	#destination will be a variable to calculate a new direction on parallel
	#That way the process 0 will have to just re-enter it in the hash but wont calculate
	#The new keys
		self.destination = home
		pass




#infects people who are infectable, if infected returns 1 to easy counting
	def infect(self, dmn, dmx):
		if self.illness_countdown == -1 and self.immunity_countdown <= 0:
			self.illness_countdown = random.randint(dmn, dmx)
			return 1
		return 0




#If an individual has -1 illness he is not infected
#if it has 0 sickness, time to decide his fate!!!!
#He'll die or get immunized and cured
#if it has immunity, then he can't get sick

#returns -1 if not necessary on calcs
#returns 0 if dies
#returns 1 if gets immunized
#return 2 if infected
	def calculate_illness(self, prj, prm, infected, piv, dmn, dmx):
		#Calculate infection
		if self.alive and self.immunity_countdown <= 0 and self.illness_countdown == -1:
			for i in range(0, infected):
				infection_roll = random.uniform(0,1)
				if infection_roll <= piv:
					self.infect(dmn, dmx)
					return 2

		#Calculate illness
		if self.alive and self.illness_countdown > 0:
			self.illness_countdown -= 1
			return 2

		#Calculate immunity
		if self.alive and self.immunity_countdown >= 0:
			self.immunity_countdown -= 1
			return 1
		
		#Calculate illness termination
		if self.alive and self.illness_countdown == 0 and self.immunity_countdown < 0:
			survival_roll = random.uniform(0, 1)
			if self.underage and survival_roll < (1 - prj) or self.underage and survival_roll < (1 - prm):
			 	self.alive = False
			 	return 0

			else:
				self.immunity_countdown = random.randint(200, 480)
				self.illness_countdown = -1
				return 1
		if not self.alive:
			return 0

		return -1





#EZ as pie, isn't it?
	def get_destination(self):
		return self.destination 





#This will calculate the individual´s displacement around a radius
#This'll use an extra index for a little bit of randomness
	def calculate_movement (self, index, tmm, rmj, rmm, vmj, vmm):
		vel = vmj if self.underage else vmm
		rad = rmj if self.underage else rmm
		direction = [(-1,0), (0,-1), (1,0), (-1,0), (-1,-1), (1,-1), (-1,1), (1,1)]

		for i in range(index, index + 8):
			index = i
			possible_destination = sumVec(self.destination, scalarMul(vel, direction[i%8]))
			if possible_destination[0] > 0 and possible_destination[0] < tmm and possible_destination[1] > 0 and possible_destination[1] < tmm:
				home_rad = sumVec(self.home, scalarMul(rad, direction[i%8]))
				if vecAbs(possible_destination)[0] < vecAbs(home_rad)[0] and vecAbs(possible_destination)[1] < vecAbs(home_rad)[1] or rmm == 0 or rmj ==0:
					self.destination = possible_destination
					break
		return index + 1



		

#Just a lot of data to simplify data logging
class Bundled_Metadata():
	
	def __init__(self, susceptible, infected, dead, immune):
		self.susceptible = susceptible
		self.infected = infected
		self.dead = dead
		self.immune = immune
		pass 
