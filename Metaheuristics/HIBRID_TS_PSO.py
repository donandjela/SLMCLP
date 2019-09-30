#!/usr/bin/env python
# coding: utf-8

# In[1]:


from problem_statement import  *
import tqdm
import time
Vmax = 200


# In[2]:


def sigmoid(x):
    return (1 / (1 + np.exp(-x)))


# In[3]:


def tabu_search(Y0, maxiter = 2000):
    
    tabu_len = int(nbI1**2*0.01)
   
    tabu_set = []
    
    Y_current = Y0
    _, current_val = cost_fja(Y_current) 
    
    best_val = current_val
    Y_best = Y_current
    
    k = 0
    i = 0
    while i < maxiter:
       
        if k > maxiter*0.2:
            Y_new = new_feasible_slower(Y_current,k = max(int(p/4),1))
        else:
            Y_new, code = new_feasible(Y_current)
        
        if code not in tabu_set:
     
            _, new_value = cost_fja(Y_new)
            i+=1
            if new_value > current_val:
               
                current_val = new_value
                Y_current = Y_new
                k=0
                if new_value> best_val:
                    tabu_set = []
                    best_val = new_value
                    Y_best = Y_new
                   
                    #print(best_val)
            else:
                  k+=1
        else:     

            if len(tabu_set) < tabu_len:
                tabu_set.append(code)
            else:
                tabu_set.pop(0)
                tabu_set.append(code)
    
    return   Y_best
    


# In[4]:


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
        
        
        global tabu_set
        tabu_set = []
        
        for i in range(num_dimensions):
            #add velocity vector
            self.velocity_i.append(random.uniform(-Vmax,Vmax))
        
        #initialize position vectors
        self.position_Y_i = Y0.copy()
        

    # evaluate current fitness for a particle
    def evaluate(self,costFunction):
        _,self.err_i = costFunction(self.position_Y_i)
        
       
        
        if self.err_i > self.err_best or self.err_best ==-1:
            
            #pos_Y_ts, err_ts = tabu_search_(self.position_Y_i, tabu_set, maxiter = 3)
            
            #if err_ts > self.err_i:
            #   self.pos_best_Y = pos_Y_ts
            #   self.err_best = err_ts
            
            
            self.pos_best_Y = self.position_Y_i
            
            self.err_best = self.err_i
        
            
        
        #if len(tabu_set)<(100):
            
        #   tabu_set.append(list(self.position_Y_i))
        
    
    def update_velocity(self, pos_best_g, w = 2, c1 = 10, c2 = 10):
        # w -> constant inertia weight (how much to weigh the previous velocity)
        # c1 -> cognative constant
        # c2 -> social constant
        
        for i in range(num_dimensions):
            r1=random.uniform(0,1)
            r2=random.uniform(0,1)
            
            #cognitive influence
            vel_cognitive=c1*r1*(self.pos_best_Y[i]-self.position_Y_i[i])
            #social influence
            vel_social=c2*r2*(pos_best_g[i]-self.position_Y_i[i])
            self.velocity_i[i]=w*self.velocity_i[i] + vel_cognitive + vel_social    
        
        
        # limit the velocity to be in (-Vmax,Vmax)
           
            self.velocity_i = np.divide(self.velocity_i, np.linalg.norm(self.velocity_i))*Vmax
        
        
    
    def update_position(self):
        
        for i in range(nbI1):
        
            sig_velocity = sigmoid(self.velocity_i[i])
            rand_val = random.random()
            
            self.position_Y_i[i] = rand_val < sig_velocity
        
        self.position_Y_i = make_Y_feasible(self.position_Y_i)
         


# In[5]:


def PSO(costFunc = cost_fja,num_particles = 30, maxiter = 100):
        
    global num_dimensions

    num_dimensions= nbI1 + nbI2
    err_best_g=-1                   # best error for group
    pos_best_g=[]                   # best position for group
    time_start = time.time()
    
    # establish the swarm
    swarm=[]
    for i in range(10):
        swarm.append(Particle(tabu_search(initialize(), maxiter = 2 * maxiter)))
    
    for i in range(10,num_particles):
        swarm.append(Particle(initialize()))

    # begin optimization loop
    
    
    for i in tqdm.tqdm(range(maxiter)):
        
        for j in range(num_particles):
           
            swarm[j].evaluate(costFunc)

            # determine if current particle is the best (globally)
            if swarm[j].err_i > err_best_g or err_best_g == -1:
                
                
                pos_best_g=list(swarm[j].position_Y_i)
                err_best_g=float(swarm[j].err_i)
                
                
                #err_best_g, pos_best_g = tabu_search(swarm[j].position_Y_i)
                
                time_best = time.time() - time_start
                

        # cycle through swarm and update velocities and position
        for j in range(0,num_particles):
            swarm[j].update_velocity(pos_best_g)
            swarm[j].update_position()
    print(err_best_g)
    time_end = time.time() - time_start
        
    
    return err_best_g, time_best, time_end
    
    


# In[6]:


#err_best_g, time_best, time_end = PSO(maxiter = 1000)


# In[ ]:





# In[7]:


#err_best_g


# In[8]:


#time_best


# In[9]:


#time_end


# In[ ]:




