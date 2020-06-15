#Gets a dictionary and makes it in a list
#At this particular case it'll remove the keys and 
#just return a list of lists
def dict2list(old_dictionary):
	new_list = []
	for key in old_dictionary:
		new_list.append(old_dictionary[key])
	return new_list





#Data logging
#Transforms a Tuple of tic data into
#Log entrys in a file
def data_logging(data, file):
	for element in data:
		pass




#Parse a list into a list of lists to send in a scatter
def chunkIt(seq, num):
    avg = len(seq) / float(num)
    out = []
    last = 0.0

    while last < len(seq):
        out.append(seq[int(last):int(last + avg)])
        last += avg

    return out





#appends an item to a dictionary somehow
def appendModel(element, model):
    key = element.get_destination()
    if key in model:
        model[key].append(element)
    else:
        model[key] = [element]





#Vectorial functions
def scalarMul(c, v):
    return (c * v[0] , c * v[1])

def vecMul(u, v):
    return (u[0] * v[0] , u[1] * v[1])

def sumVec(u, v):
    return (u[0] + v[0] , u[1] + v[1])

def vecAbs(u):
    x = u[0] if u[0] > 0 else u[0] * -1
    y = u[1] if u[1] > 0 else u[1] * -1
    return (x,y)