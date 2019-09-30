
# coding: utf-8

# In[1]:


from problem_statement import  *
import tqdm
import time


# In[2]:


def tabu_search(Y0 = initialize(), maxiter = 10000):
    
    tabu_len = int(nbI1**2*0.05)
   
    tabu_set = []
    
    Y_current = Y0
    _, current_val = cost_fja(Y_current) 
    
    best_val = current_val
    Y_best = Y_current
    
    k = 0
    i = 0
    
    time_start = time.time()
    time_best = 0
    indikator = None
    while i < maxiter:
        
        if k > maxiter*0.2:
            Y_new = new_feasible_slower(Y_current, k = max(int(p/4),1))
            indikator = True
        else:
            Y_new, code = new_feasible(Y_current)
            indikator = False
        
        if code not in tabu_set or indikator:
     
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
                    time_best = time.time() - time_start
                   
                    #print(best_val)
            else:
                  k+=1
        else:     

            if len(tabu_set) < tabu_len:
                tabu_set.append(code)
            else:
                tabu_set.pop(0)
                tabu_set.append(code)
                
    time_end = time.time() - time_start
    
    return   best_val, time_best, time_end
    

