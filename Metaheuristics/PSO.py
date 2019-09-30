#!/usr/bin/env python
# coding: utf-8

# In[1]:


#from read_data import *
from problem_statement import *
import tqdm
import time

Vmax = 2000


# In[2]:


def sigmoid(x):
    return np.divide(1, 1+ np.exp(-x))


# In[3]:


class Particle():
    
    #initiliaze particle
    def __init__(self, Y0):
        #individual parameters
        self.position_Y_i = [] #particle position // matrix Y
        
        self.velocity_i = [] #particle velosity
        self.err_i= -1       #error individual
        
        #global parameters
        
        self.pos_best_Y = [] #best position individual Y
        
        self.err_best = -1   #best error individual
        
        for i in range(num_dimensions):
            #add velocity vector
            self.velocity_i.append(random.uniform(-Vmax,Vmax))
        
        #initialize position vectors
        self.position_Y_i = Y0.copy()
        

    # evaluate current fitness for a particle
    def evaluate(self,costFunction):
        X, self.err_i = costFunction(self.position_Y_i)
        
        # check to see if the current position is
        # new global best position
        if self.err_i > self.err_best or self.err_best ==-1:
            self.pos_best_Y = self.position_Y_i
    
            self.err_best = self.err_i
        
    
    def update_velocity(self, pos_best_g, w = 10, c1 = 100, c2 = 100 ):
        # w -> constant inertia weight (how much to weigh the previous velocity)
        # c1 -> cognative constant
        # c2 -> social constant
        
        vel_max = -np.inf
        
        for i in range(num_dimensions):
            r1=random.uniform(0,1)
            r2=random.uniform(0,1)
            
            #cognitive influence
            vel_cognitive=c1*r1*(self.pos_best_Y[i]-self.position_Y_i[i])
            #social influence
            vel_social=c2*r2*(pos_best_g[i]-self.position_Y_i[i])
            self.velocity_i[i]=w*self.velocity_i[i] + vel_cognitive + vel_social
      
        
        self.velocity_i = np.divide(self.velocity_i, np.linalg.norm(self.velocity_i))*Vmax
        
    
        
    def update_position(self):
        
        for i in range(nbI1):
            
            self.position_Y_i[i]= random.random() < sigmoid(self.velocity_i[i])
            
        self.position_Y_i = make_Y_feasible(self.position_Y_i)
                        


# In[4]:



def PSO(costFunc = cost_fja,num_particles = 50 ,maxiter = 2000):
        
        
    global num_dimensions
    num_dimensions= nbI1 + nbI2
    err_best_g=-1                   # best error for group
    pos_best_g=[]                   # best position for group

        # establish the swarm
    swarm=[]
    for i in range(num_particles):
        swarm.append(Particle(initialize()))

        # begin optimization loop
        
    time_start = time.time()
    time_end = None
    for i in tqdm.tqdm(range(maxiter)):
            
        #print i,err_best_g
        # cycle through particles in swarm and evaluate fitness
        for j in range(num_particles):
            swarm[j].evaluate(costFunc)

            # determine if current particle is the best (globally)
            if swarm[j].err_i > err_best_g or err_best_g == -1:
                pos_best_g=list(swarm[j].position_Y_i)
                err_best_g=float(swarm[j].err_i)
                time_best = time.time() - time_start

        # cycle through swarm and update velocities and position
        #for j in range(0,num_particles):
            swarm[j].update_velocity(pos_best_g)
            swarm[j].update_position()
                
        
    time_end = time.time() - time_start
        
    return err_best_g, time_best, time_end


# In[ ]:




