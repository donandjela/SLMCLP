#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ilcplex/cplex.h>
#define DATA "rez.csv"
#define IN "instance/mclp_S4.txt"


// Tacka unutar kruga
int isInside(double x1, double y1, double x2, double y2, double r)
{
    double xSqr = (x1 - x2) * (x1 - x2);
    double ySqr = (y1 - y2) * (y1 - y2);
    if (xSqr + ySqr <= r * r)
        return 1;
    return 0;
}
void error(const char* message, CPXENVptr env = NULL, CPXLPptr lp = NULL)
{
    printf("%s.\n", message);
    if (env != NULL && lp != NULL) CPXfreeprob(env, &lp);
    if (env != NULL) CPXcloseCPLEX(&env);
    exit(0);
}

void allocate(double** rhs, char** sense, int** matbeg, int** matind, double** matval, int n, CPXENVptr env, CPXLPptr lp)
{
    *rhs = (double*) malloc(sizeof(double));
    if (*rhs == NULL) error("Memory allocation problem", env, lp);
    *sense = (char*) malloc(sizeof(char));
    if (*sense == NULL) error("Memory allocation problem", env, lp);
    *matbeg = (int*) malloc(sizeof(int));
    if (*matbeg == NULL) error("Memory allocation problem", env, lp);
    *matind = (int*) malloc(n * sizeof(int));
	if (*matind == NULL) error("Memory allocation problem", env,lp);
    *matval = (double*) malloc(n * sizeof(double));
    if (*matval == NULL) error("Memory allocation problem", env, lp);
}


void free_(double** rhs, char** sense, int** matbeg, int** matind, double** matval)
{
    free(*rhs);
    free(*sense);
    free(*matbeg);
    free(*matind);
    free(*matval);
}


int num_before_k(int* niz, int k){
    int sum = 0;
    for (int i = 0; i<k; i++){
        sum = sum + (niz[i]);
    }

    return sum ;

}

int find_ind(int* niz, int n, int vred){

    for (int i=0; i<n; i++){
       // printf("%d  == %d \n", niz[i], vred);
        if (niz[i] == vred)
            return i;
    }
return -1;
}

typedef struct xij{
	int i;
	int j;
} spars;


int pronadji_matind(spars* xij,int n, int i, int j){

for (int k = 0; k<n; k++){

if (xij[k].i == i && xij[k].j ==j)
return k;

}
return -1;

}


void pisi(char* instanca,int p, double radius, int n_customers, int o_customers, int n_facilities, int o_facilities,
		double rezultat, double vreme, int iteracija, int cvorovi, double gap){

FILE *f = fopen(DATA,"a");
if (f == NULL){
printf("Ne otvara se fajl za ispis \n");
exit(1);
} 


fprintf(f,"%s %d %f %d %d %d %d %f %f %d %d %f",instanca,p, radius, n_customers, o_customers, n_facilities, o_facilities, rezultat,vreme,iteracija,cvorovi,gap);
fprintf(f,"\n");

fclose(f);
}








int main()
{
    // Pocetno vreme
    double startTime = clock();

    printf("Pocinje program \n ");
    FILE* in = fopen(IN, "r");
    if (in == NULL) printf("Can't open input file");

    // Ulazne velicine

    //radius
    double radius;
    fscanf(in, "%lf", &radius);
    printf("radius= %f \n", radius);

    //broj ustanova koje lociramo -> p
    int p;
    fscanf(in, "%d", &p);
    printf("p = %d \n", p);



	int nbI2;
	int nbI1;

	int nbJ1;
	int nbJ2;

    fscanf(in,"%d",&nbJ1);
    fscanf(in,"%d",&nbJ2);

    fscanf(in,"%d",&nbI1);
    fscanf(in,"%d",&nbI2);

    printf("nbJ1, nbJ2, nbI1, nbI2 = %d  %d  %d %d  \n",nbJ1, nbJ2, nbI1, nbI2);

    int nbI = nbI1 + nbI2;
    int nbJ = nbJ1 + nbJ2;

    int all = nbI + nbJ;

    double* x = (double*) malloc(all * sizeof(double));
    double* y = (double*) malloc(all * sizeof(double));
    for (int i = 0; i < all; i++) {
        fscanf(in, "%lf%lf", &x[i], &y[i]);
       // printf("%lf  %lf \n",x[i],y[i]);
    }

    int **pref = (int**) malloc(nbI*sizeof(int*));

    for (int i = 0; i<nbI; i++){

        pref[i] = (int*) malloc((nbJ)*sizeof(int));

        for (int j = 0; j<nbJ;j++){
            fscanf(in,"%d",&pref[i][j]);
          //  printf("%d ", pref[i][j]);
        }
     //   printf("\n");
    }


    // potraznja kupaca
    printf("Potraznja kupaca \n");
    double* potraznja = (double*) malloc(nbJ * sizeof(double));
    for (int i = 0; i < nbJ; i++) {
        fscanf(in, "%lf", &potraznja[i]);
       // printf("%lf   ", potraznja[i]);
    }


    //printf("\n Kupci: \n");
    int* customers = (int*) malloc(nbJ * sizeof(int));

    // Kupci

    for (int j = 0; j < nbJ; j++) {
        fscanf(in, "%d", &customers[j]);
      //  printf("%d ", customers[j]);
    }

    printf("\n");

//Facilities

    //printf("Lokacije \n");
    int* facilities = (int*) malloc(nbI * sizeof(int));

    for (int i = 0; i < nbI; i++) {
        fscanf(in, "%d", &facilities[i]);
      //  printf("%d ", facilities[i]);
    }
     //printf("\n");


    //printf("Skupovi \n");

	int* nb_J_i = (int*) malloc(nbI * sizeof(int));
	int** J_i = (int**) malloc(nbI * sizeof(int*));
    	int nb_all_J_i = 0;

    printf("nbJ  %d  \n",nbJ);
    printf("nbI  %d  \n",nbI);



printf("*********** \n");

    for (int i = 0; i<nbI; i++){

    nb_J_i[i] = 0;
    for (int j=0; j<nbJ; j++){
        if (isInside(x[facilities[i]],y[facilities[i]],x[customers[j]], y[customers[j]], radius))
            nb_J_i[i]++;

    }

    nb_all_J_i +=nb_J_i[i];


    J_i[i] = (int*) malloc(nb_J_i[i]*sizeof(int));
    nb_J_i[i] = 0;
    for (int j=0; j<nbJ; j++){
        if (isInside(x[facilities[i]],y[facilities[i]],x[customers[j]], y[customers[j]], radius)){
            J_i[i][nb_J_i[i]++] = j;
          //  printf("%d ", j);




        }
    }

   // printf("\n");

    }



spars* xij = (spars*) malloc(nb_all_J_i * sizeof(spars));

int br = 0;


for (int i = 0; i<nbI; i++){
	for (int j =0; j<nb_J_i[i]; j++){

	xij[br].i = i;
	xij[br].j = J_i[i][j];

//printf("x_%d,%d  ",i, J_i[i][j]);
	br++;}
//printf("\n");
}

int number_xij = br;
//printf("number xij = %d \n",number_xij);

//printf("*********** \n");


int* nb_I_j = (int*) malloc(nbJ * sizeof(int));
int** I_j = (int**) malloc(nbJ * sizeof(int*));

//printf("*********** \n");

    for (int j = 0; j<nbJ; j++){

    nb_I_j[j] = 0;
    for (int i=0; i<nbI; i++){
        if (isInside(x[facilities[i]],y[facilities[i]],x[customers[j]], y[customers[j]], radius))
            nb_I_j[j]++;

    }

    I_j[j] = (int*) malloc(nb_I_j[j]*sizeof(int));
    nb_I_j[j] = 0;
    for (int i=0; i<nbI; i++){
        if (isInside(x[facilities[i]],y[facilities[i]],x[customers[j]], y[customers[j]], radius)){
            I_j[j][nb_I_j[j]++] = i;
  //          printf("%d ", i);
        }
    }

  //  printf("\n");

    }
//printf("*********** \n");


// "Sortirana matrica preferenci (indeksi)  G"
// Zapravo samo menjam vec postojecu I_j,sortiram elemente tako da odgovaraju preferencama


//printf("---- FORMIRAM G------- \n");

int** G = (int**) malloc(nbJ*sizeof(int*));
int count;
for (int j = 0; j<nbJ; j++){
    G[j] = (int*) malloc(nb_I_j[j] * sizeof(int));

    count = 0;
    for( int i = 0; i<nbI; i++){

	//printf("ulazi u find_ind: %d \n",pref[i][j]);
        int indikator = find_ind(I_j[j],nb_I_j[j],pref[i][j]);

	//printf("%d \n", indikator);


        if (count>nb_I_j[j]) break;
        if (indikator != -1){
           G[j][count++] = pref[i][j];

  //          printf(" %d ", pref[i][j]);
        }


    }
    // printf("\n");

}


//printf("---- FORMIRAM G------- \n");




fclose(in);

   // Otvaranje CPLEX-a
    int status;
    CPXENVptr env = CPXopenCPLEX(&status);
    if (status) error("Can't open CPLEX");

    // Kreiranje problema
    CPXLPptr lp = CPXcreateprob(env, &status, "MCLP");
    if (status) error("Can't create problem", env);

    // Maksimizacija ili manimizacija?
    status = CPXchgobjsen(env, lp, CPX_MAX);
    if (status) error("Can't change sense", env, lp);



double* coeffs = (double*) malloc((nbI + nb_all_J_i) * sizeof(double));
if (coeffs == NULL) error("Memory allocation problem", env, lp);
double* lowerBound = (double*) malloc((nbI + nb_all_J_i) * sizeof(double));
if (lowerBound == NULL) error("Memory allocation problem", env, lp);
double* upperBound = (double*) malloc((nbI + nb_all_J_i) * sizeof(double));
if (upperBound == NULL) error("Memory allocation problem", env, lp);
char* type = (char*) malloc((nbI + nb_all_J_i) * sizeof(char));
if (type == NULL) error("Memory allocation problem", env, lp);

//Funkcija cilja i def ogranicenja:

//Yj

for (int i=0; i<nbI; i++){
	coeffs[i] = 0.0;
	lowerBound[i] = 0.0;
	upperBound[i] = 1.0;
	type[i] = 'B';
}

//Xij

int counter = 0;

for(int i=0; i<nbI1; i++){
	for (int j = 0; j<nb_J_i[i]; j++){

	coeffs[nbI + counter] = potraznja[J_i[i][j]];
	lowerBound[nbI + counter] = 0.0;
	upperBound[nbI + counter] = 1.0;
	type[nbI + counter] = 'B';
	counter++;

	}
}

for(int i=nbI1; i<nbI; i++){
	for (int j = 0; j<nb_J_i[i]; j++){

	coeffs[nbI + counter] = 0;
	lowerBound[nbI + counter] = 0.0;
	upperBound[nbI + counter] = 1.0;
	type[nbI + counter] = 'B';
	counter++;

	}
}



int num_Xij = counter;

//printf("num_Xij == nb_all_J_i = %d \n", num_Xij==nb_all_J_i);
/*
for (int k = 0; k < nbI + counter; k++){

	printf(" coef[%d]: %.2f \n",k, coeffs[k]);

}
*/
printf("Ukupno koeficijenata: %d \n",nbI + nb_all_J_i);

/*Prethodano radi dobro :D */

// Dodavanje promenljivih

status = CPXnewcols(env, lp, nbI + nb_all_J_i, coeffs, lowerBound, upperBound, type, NULL);
if (status) error("Can't add new columns", env, lp);

    // Ogranicenja

    double* rhs;
    char* sense;
    int* matbeg;
    int* matind;
    double* matval;

//Ogranicenje (2)


printf("====== (2) ======= \n");

for (int i = nbI1; i<nbI; i++){

	allocate(&rhs, &sense, &matbeg, &matind, &matval, 1, env, lp);

	rhs[0] = 1.0;
	sense[0] = 'E';
	matbeg[0] = 0;

	matind[0] = i;
	matval[0] = 1.0;

	//printf("%d  \n", matind[0]);

	
	status = CPXaddrows(env, lp, 0, 1, 1, rhs, sense, matbeg, matind, matval, NULL, NULL);
    if (status) error("Can't add new rows", env, lp);
    
    free_(&rhs, &sense, &matbeg, &matind, &matval);

}
printf("====== (2) ======= \n");





printf("====== (3) ======= \n");



//Ogranicenje (3)

allocate(&rhs, &sense, &matbeg, &matind, &matval, nbI1, env, lp);

rhs[0] = p;
sense[0] = 'E';
matbeg[0] = 0;
for (int i =0; i<nbI1; i++){

	matind[i] = i;
	matval[i] = 1.0;

	//printf("%d   ", matind[i]);

}
//printf("\n");

status = CPXaddrows(env, lp, 0, 1, nbI1, rhs, sense, matbeg, matind, matval, NULL, NULL);
if (status) error("Can't add new rows", env, lp);

    free_(&rhs, &sense, &matbeg, &matind, &matval);


printf("====== (3) ======= \n");



printf("====== (6) ======= \n");




//Ogranicenje (6)



for (int j = nbJ1; j<nbJ; j++ ){
	allocate(&rhs, &sense, &matbeg, &matind, &matval, nb_I_j[j], env, lp);

	rhs[0] = 1.0;
	sense[0] = 'E';
	matbeg[0] = 0;

    counter = 0;
	for (int i =0; i< nb_I_j[j]; i++){
	
	int indeks = pronadji_matind(xij,number_xij,I_j[j][i],j);
	//printf("indeks = %d \n", indeks);

        matind[i] = indeks + nbI;
	matval[i] = 1.0;
	//printf(" %d.  %d  \n", i, matind[i]);
	}
	//printf("dodajem %d \n", nb_I_j[j]);
	
	status = CPXaddrows(env, lp, 0, 1, nb_I_j[j], rhs, sense, matbeg, matind, matval, NULL, NULL);
    if (status) error("Can't add new rows", env, lp);

    //printf("\n");
    free_(&rhs, &sense, &matbeg, &matind, &matval);


}

printf("====== (6) ======= \n");


printf("====== (7) ======= \n");



//Ogranicenje (7)

counter = 0;
for (int i=0; i<nbI; i++ ){



	for (int j=0; j<nb_J_i[i]; j++ ){

	allocate(&rhs, &sense, &matbeg, &matind, &matval, 2, env, lp);

	rhs[0] = 0.0;
	sense[0] = 'L';
	matbeg[0] = 0;

	matind[0] = i;
	matval[0] = -1.0;

	matind[1] =  nbI + counter;
	matval[1] = 1.0;


	//printf("y:%d   x: %d \n", matind[0], matind[1]);


	counter++;

	status = CPXaddrows(env, lp, 0, 1, 2, rhs, sense, matbeg, matind, matval, NULL, NULL);
	if (status) error("Can't add new rows", env, lp);

	free_(&rhs, &sense, &matbeg, &matind, &matval);

	}
}


printf("====== (7) ======= \n");





printf("====== (8) ======= \n");


//Ogranicenje (8)


counter = 0;
for (int j = 0; j<nbJ1; j++ ){
	allocate(&rhs, &sense, &matbeg, &matind, &matval, nb_I_j[j], env, lp);

	rhs[0] = 1.0;
	sense[0] = 'L';
	matbeg[0] = 0;


	for (int i =0; i< nb_I_j[j]; i++){
        //printf("counter:  %d \n", counter);


        int indeks = pronadji_matind(xij,number_xij,I_j[j][i],j);
	//printf("indeks = %d \n", indeks);

        	matind[i] = indeks + nbI;
		matval[i] = 1.0;

	//	printf("%d.  %d \n", i,matind[i]);


	}
		counter++;
	//printf("dodam %d prom \n",nb_I_j[j]);
	
	status = CPXaddrows(env, lp, 0, 1, nb_I_j[j], rhs, sense, matbeg, matind, matval, NULL, NULL);
    if (status) error("Can't add new rows", env, lp);

    //printf("\n");
    free_(&rhs, &sense, &matbeg, &matind, &matval);


}
printf("====== (8) ======= \n");




printf("====== (10) ======= \n");

for (int j = 0; j<nbJ; j++){
    for (int k = 0; k<nb_I_j[j]-1; k++){

    //!!!!!!!!!!!!!!!!!11 izmeni ovo !!!!!!!!!!!!!
    allocate(&rhs, &sense, &matbeg, &matind, &matval, nb_I_j[j]-k, env, lp);


    	rhs[0] = 1.0;
	sense[0] = 'L';
	matbeg[0] = 0;

	matind[0] = G[j][k];
	matval[0] = 1.0;
    	//printf("matind[0]: %d \n", matind[0]);

    for (int i= 0; i<nb_I_j[j]-k-1; i++){

    	int ind = G[j][i+k+1];

	int indeks = pronadji_matind(xij,number_xij,ind,j);
	//printf("indeks = %d \n", indeks);

        matind[i+1] = indeks + nbI;
	matval[i+1] = 1.0;


    //printf("matinf[%d] = %d \n",i+1, matind[i+1]);

   // printf("matind[%d]: %d \n", i,matind[i]);
    }

    
	status = CPXaddrows(env, lp, 0, 1, nb_I_j[j]-k, rhs, sense, matbeg, matind, matval, NULL, NULL);
    if (status) error("Can't add new rows", env, lp);
  //printf("Dodajem %d prom \n", nb_I_j[j]-k);
    //printf("\n");
    free_(&rhs, &sense, &matbeg, &matind, &matval);


    }

}

printf("====== (10) ======= \n");

// promenim default vrednost za relmipgap:
status = CPXsetdblparam(env, CPX_PARAM_EPGAP,0.0 );
if(status) error("Can't set gap tolerance",env,lp);





//Maksimalno vreme izvrsavanja

status = CPXsetdblparam(env, CPX_PARAM_TILIM,7200.0 );
if(status) error("Can't set time limit",env,lp);


// Podesavanje tipa problema
    status = CPXchgprobtype(env, lp, CPXPROB_MILP);
    if (status) error("Can't change problem type", env, lp);



    // Optimizacija
    status = CPXmipopt(env, lp);
    if (status) error("Can't optimize", env, lp);



    // Resenje i njegova vrednost
    double objval;
    status = CPXgetobjval(env, lp, &objval);
    if (status) error("Can't obtain solution!!!", env, lp);

	
    double* solution = (double*) malloc((nbI + nb_all_J_i) * sizeof(double));
    status = CPXgetx(env, lp, solution, 0, CPXgetnumcols(env, lp) - 1);
    if (status) error("Can't obtain solution vector", env, lp);
    
    // Broj cvorova, iteracija i gap
    int nodes = CPXgetnodecnt(env, lp);
    int iters = CPXgetmipitcnt(env, lp);
    double gap;
    status = CPXgetmiprelgap(env, lp, &gap);
    if (status) error("Can't obtain gap", env, lp);

    // Ukupno vreme izvrsavanja
    double endTime = clock();
    double timeInSeconds = (endTime - startTime) / CLOCKS_PER_SEC;



pisi(IN,p, radius, nbJ1, nbJ2, nbI1, nbI2, objval, timeInSeconds, iters, nodes, gap);



//!!!!!!!!!!! ispis rezultata
	printf("object value: %lf \n",objval);
	printf("time in seconds: %lf \n",timeInSeconds);
	printf("nodes %d \n", nodes);
	printf("iters %d \n", iters);
	printf("gap: %lf \n",gap);

/*
	for (int i=0; i<nbI + nb_all_J_i; i++){
	printf("solution[%d] = %f \n", i, solution[i]);
}
*/

	// Zatvaranje problema
    if (env != NULL && lp != NULL) {
        CPXfreeprob(env, &lp);
        if (status) error("Can't free problem", env);
    }

    // Zatvaranje CPLEX-a
    if (env != NULL) {
        CPXcloseCPLEX(&env);
        if (status) error("Can't close CPLEX");
    
    }




	
	
	



    return 0;
}
