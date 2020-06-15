import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt


#Just plots the data from an array of tuples
#the tuples got percent of succeptible, infected, deceased and immune of a tic
def graph(daily_stats) :
    days = []
    susceptible = []    
    infected = []
    death = []
    inmune = []


    for i in range(0,len(daily_stats)):
        s, inf, d, imn = daily_stats[i]
        days.append(i)
        susceptible.append(int(s))
        infected.append(int(inf))
        death.append(int(d))
        inmune.append(int(imn))

    plt.plot(days, susceptible, label= 'Suceptibles', color= 'y')
    plt.plot(days, infected, label='Infectados', color='r')
    plt.plot(days, death, label='Fallecidos', color='k')
    plt.plot(days, inmune, label='Inmunes', color='g')

    plt.xlabel('Días trancurridos')
    plt.ylabel('Cantidad de personas')
    plt.title('Estadísticas del virus')
    plt.legend()
    plt.savefig('Resultados.png')




