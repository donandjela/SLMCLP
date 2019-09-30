
# coding: utf-8

# In[1]:


from time import sleep
import sys
import numpy as np
import math
import random
from read_data import *


# In[2]:


#[OK]
def make_Y_feasible(Y):
   
    suma = int(np.sum(Y[:nbI1]))
    
    if suma == p:
        return Y
    
    if suma < p:
        indices_0 = [i for i in range(nbI1) if Y[i]==0]
        index_0 = np.random.choice(indices_0, p-suma, replace = False)
        Y[index_0] = 1
        
    if suma > p:
        
        indices_1 = [i for i in range(nbI1) if Y[i]== 1]
        index_1 = np.random.choice(indices_1, suma - p, replace= False)
        Y[index_1] = 0
        

    assert int(np.sum(Y[:nbI1])) == p
    
    return Y  
    


# In[3]:


#[OK]
def new_feasible_slower(Y, k = 2):
    
    if p==nbI1:
        return Y
    
    
    indices_1 = [i for i in range(nbI1) if Y[i]==1 ]
    indices_0 = [i for i in range(nbI1) if Y[i]==0 ]
    
    if p<=k:
        k = min(len(indices_1), len(indices_0))
    
    index_0 = np.random.choice(indices_0,k, replace = False)
    index_1 = np.random.choice(indices_1, k , replace= False)
    
    Y[index_1] = 0
    Y[index_0] = 1
    
    assert int(np.sum(Y[:nbI1])) == p
    
    return Y    


# In[4]:


#[OK]
def new_feasible(Y):
    
    if p==nbI1:
        return Y, (-1,-1)
    
    indices_1 = [i for i in range(nbI1) if Y[i]==1 ]
    indices_0 = [i for i in range(nbI1) if Y[i]==0 ]
    
    index_1 = np.random.randint(0, len(indices_1))
    index_0 = np.random.randint(0, len(indices_0))
    
    Y[indices_1[index_1]] = 0
    Y[indices_0[index_0]] = 1
    a = (index_1, index_0)
    assert int(np.sum(Y[:nbI1])) == p
    
    
    return Y, a


# In[5]:


#[OK]
def initialize():
    
    Y = np.zeros(nbI1+nbI2)
    Y[nbI1:] = 1
    
    
    ind =np.random.choice(range(nbI1), replace = False, size = p )
    
    
    Y[ind] = 1

    Y,_ = new_feasible(Y)
    return Y


# In[6]:


#[OK]
def cost_fja(Y):
    
    cost = 0
    X = None
    
    for j in range(nbJ1 + nbJ2):
        
        for ind in G[j]:
            i = I_j[j][ind]
                
            if Y[i]:
                    if i<nbI1:
                        cost += D[j]
                    break
            
    return X,cost


# In[7]:

