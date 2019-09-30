
# coding: utf-8

# In[1]:


import numpy as np


# In[2]:



#All files
example = 'Instance//mclp_S6.txt'
small = ['mclp_S'+str(i) + '.txt' for i in range(1,20)]
medium = ['mclp_M'+str(i) + '.txt' for i in range(1,20)]
large = ['mclp_L'+str(i) + '.txt' for i in range(1,20)]


# In[3]:


def dist(A,B):
    x1,y1 = A
    x2,y2 = B
    return np.sqrt((x1-x2)**2 + (y1-y2)**2)


# In[4]:


def cost_func(D, I_1, J_i,X):
    func_value = 0
    k = 0
    for i in range(len(I)):
        for j in J_i[i]:
            func_value += D[j]*X[k]
            print(D[j])
            k+=1
    return func_value


# In[5]:



def form_sets(points,I,J, radius):
    mat = []

    #print("I(j)")

    for j in range(len(J)):
        pom = []
        for i in range(len(I)):

            if dist(points[J[j]],points[I[i]])<=radius:
                pom.append(i)
        mat.append(pom)

    #print(mat)

    I_j = mat
    mat = []

    #print("J(i)") 
    for i in range(len(I)):
        pom = []
        for j in range(len(J)):

            if dist(points[J[j]],points[I[i]])<=radius:
                pom.append(j)
        mat.append(pom)

    #print(mat)
    J_i = mat
            
    return (I_j,J_i)


# In[6]:


def read_input(instance):
  
    f = open(instance, 'r')
    
    #radius
    radius = int(f.readline().strip('\n'))
    #print(radius)
    
    #p
    p = int(f.readline().strip('\n'))
    #print(f'p = {p}')
    
    #nbI1 -> number of 
    nbJ1 = int(f.readline().strip('\n'))
    #print(f'nbJ1 = {nbJ1}')
    
    #nbI2 -> number of 
    nbJ2 = int(f.readline().strip('\n'))
    #print(f'nbJ2 = {nbJ2}')
    
    #nbI1 -> number of 
    nbI1 = int(f.readline().strip('\n'))
    #print(f'nbI1 = {nbI1}')
    
    #nbJ2 -> number of 
    nbI2 = int(f.readline().strip('\n'))
    #print(f'nbI2 = {nbI2}')
    
    f.readline()
    
    nbI = nbI1 + nbI2
    nbJ = nbJ1 + nbJ2
    all_IJ = nbI + nbJ
    
    coordinates = []
    
    for i in range(all_IJ):
        line = f.readline().strip('\n').split()
        coordinates.append([int(line[0]), int(line[1])])
    coordinates = np.array(coordinates)
    #print(coordinates)
    
    f.readline()
    
    pref = []
    line = f.readline().strip('\n').split()
    #print(line)
    #for i in range(nbI):
    #   if i%nbJ == 0:
    #       pref = []
        
    #   pref.append([int(x) for x in line])
        

    for i in range(nbI):
        pom=[]
        for j in range(nbJ):
            pom.append(int(line[i*nbJ+j]))
        pref.append(pom)
    pref = np.array(pref)
    
    #print(pref)
    
    #f.readline()
    line = f.readline().strip('\n').split()
    D = np.array([int(x) for x in line])
    #print(D)
    
    f.readline()
    line = f.readline().strip('\n').split()
    J = np.array([int(x) for x in line])
    #print(J)
    
    line = f.readline().strip('\n').split()
    I = np.array([int(x) for x in line])
    #print(I)
    
    
    return radius,p,nbI1, nbI2,I, nbJ1, nbJ2,J, coordinates, pref, D


# In[7]:


def nb_variables(I,J_i):
    sum = 0
    for i in range(len(I)):
        sum+=len(J_i[i])  
    return sum
    


# In[8]:


def variables(I,J_i):
    
    coeffs = ['Y' + str(i) for i in range(len(I))]
    
    for i in range(len(I)):
        for j in J_i[i]:
            #print(f"X_{i}{j} ")
            coeffs.append('X_' + str(i)+""+ str(j) )
        #print(),
    
    #print(coeffs)
    return coeffs
        


# In[9]:


def get_coeff_D(D,J_i):
    coeffs = list(np.zeros(len(I)))
    
    
    for elem in J_i:
        for j in elem:
            coeffs.append(D[j])
    #print(coeffs)
    
    return coeffs
    


# In[10]:


def find_ind(lis,value):
    
    for i,elem in enumerate(lis):
        if value == elem:
            return i
    return -1


# In[11]:


def form_G():
    nb_J = nbJ1 + nbJ2
    nb_I = nbI1 + nbI2
    G = []
    for j in range(nb_J):
        tmp = []
        for i in range(nb_I):
            indikator = find_ind(I_j[j],pref[i][j])

            if indikator>=0:
                tmp.append(pref[i][j])
        G.append(tmp)
                
        
        
    #print(G)
    return G
    
    


# In[12]:


global radius,p,nbI1, nbI2,I, nbJ1, nbJ2,J, coordinates, pref, D
global I_j, J_i
radius,p,nbI1, nbI2,I, nbJ1, nbJ2,J, coordinates, pref, D = read_input(example)

I_j, J_i = form_sets(coordinates,I,J, radius)

#print(p)

#print(f"broj varijabli: {nb_variables(I,J_i)}")
global N
N = variables(I,J_i)

#print(I)
#print(nbI1)
#print(I[:nbI1])
#print(J_i)

global parameters
parameters = get_coeff_D(D,J_i)


# In[13]:


def find_ind(lis,value):
    
    for i,elem in enumerate(lis):
        if value == elem:
            return i
    return -1


def form_G():
    nb_J = nbJ1 + nbJ2
    nb_I = nbI1 + nbI2

    G = []
    for j in range(nb_J):
        tmp = []
        for i in range(nb_I):
            #print(j,i,J_i[j])
            #print(pref[i][j])
            indikator = find_ind(I_j[j],pref[i][j])

            if indikator>=0:
                tmp.append(indikator)
        G.append(tmp)
                
        
        
    #print(G)
    return G
    
    


# In[14]:


def pisi_proba():
    print('PROBA!!!')


# In[15]:


def costFunction_vec(Y, params = parameters):
    
    if params!= None:
        return np.dot(params,Y)
    
    return "OK"


# In[16]:


global G

G = form_G()


# In[17]:


def stampaj():


    print('Instanca = mclp_S6.txt')
    print(f'p = {p}')
    print(f'radius = {radius}')
    print(f'nbI1 = {nbI1}')
    print(f'nbI2 = {nbI2}')
    print(f'nbJ1 = {nbJ1}')
    print(f'nbJ2 = {nbJ2}')
    print()
    print(f'I(j) = {I_j}')
    #print(f'J(i) = {J_i}')


# In[18]:

