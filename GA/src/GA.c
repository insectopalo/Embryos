/*
   Very simple genetic algorithm implementation for a toy model. The organisms
   are n-bits computer words. The fitness function is the number of ones in the
   word.
   The population is produced by initializing m words randomly. Then,
   iteratively evaluate the fitness of each vector, keep the b vectors with the
   highest values of it and apply 1-point corssover to these, producing b new
   words -the parents are chosen randomly, there's no competition-. The new b
   words substitute the bottom b words in the fitness-sorted population.
   Break the loop when bmax generations have been run or when a word reaches a
   fitness of 1.

   Author: Gonzalo S Nido <insectopalo@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>

/* 
   Definition of the constants used in the simulation.
   - G_SIZE (n) is the length in bits of each computer word.
   - P_SIZE (m) is the number of words in the population
   - BOTTLENECK (b) is the number of genomes that survive each generation giving
     rise to the progeny by crossover
   - MAX_GENERATIONS (bmax) is the maximum number of generations in the
     simulation 
*/
#define G_SIZE 16
#define P_SIZE 40
#define BOTTLENECK 20
#define MAX_GENERATIONS 10

/*
   Function to generate a random population. It is just a random binary matrix
   of G_SIZE x P_SIZE (rand() % 2). The matrix must be initialized beforehand,
   and iven as the argument to the funcion
*/

void rand_population(int p[P_SIZE][G_SIZE]) {
   srand( (unsigned int) time( NULL ));
   int i,j;
   for (i=0;i<P_SIZE;i++){
      for (j=0;j<G_SIZE;j++){
         p[i][j] = rand() % 2;
      }
   }
}


/*
   Function to calculate the fitness (between 0 and 1) of a given binary
   vector. The pointer to the binary vector is given as argument to the
   function
*/

float fitness(int * genome){
   float f=0;
   int j;
   for (j=0;j<G_SIZE;j++){
      f += genome[j];
   }
   f/=G_SIZE;
   return f;
}


/*
   Function to print the population matrix with the fitness corresponding to
   each of the words 
*/

void print_population (int p[P_SIZE][G_SIZE]){
   int i,j;
   for (i=0;i<P_SIZE;i++){
      for (j=0;j<G_SIZE;j++){
         printf ("%i",p[i][j]);
      }
      printf (" f=%.4f\n",fitness(p[i]));
   }
}


/*
   Function to do a crossover between two parents generating two descendants.
   The pointers to the population matrix lines corresponding to the parents and
   "dying" words must be given as arguments, along with an integer which
   corresponds to the point where the crossover happens
*/

void crossover(int * parent1, int * parent2, int * offspring1,
               int * offspring2, int c_point){
   int j;
   for (j=0;j<c_point;j++){
      offspring1[j] = parent1[j];
      offspring2[j] = parent2[j];
   }
   for (j=c_point;j<G_SIZE;j++){
      offspring1[j] = parent2[j];
      offspring2[j] = parent1[j];
   }
}


/*
   Functions to sort the words in the population according to their respective
   fitness. The function "comp" is used internally in sort_P_by_fitness, which
   is an implementation of the Quicksort algorithm
*/   

int comp(const void *a, const void *b){
   float diff = fitness((int *)a) - fitness((int *)b);
   printf ("%.4f vs %.4f\n",fitness((int *)a),fitness((int *)b));
   if (diff == 0){
      return 0;
   }else if (diff < 0){
      return 1;
   }else{
      return -1;
   }
}

void sort_P_by_fitness(int p[P_SIZE][G_SIZE]){
   qsort(p, P_SIZE, sizeof(int [G_SIZE]), comp);
}


/*
   Randomly shuffle an array. This function is an implementation of the
   modern version of the Fisher-Yates shuffle
*/

void shuffle_array(int * a, int n){
   int i;
   for (i=n-1;i>0;i--){
      int j = rand() % (i+1);
      int temp = a[j];
      a[j] = a[i];
      a[i] = temp;      
    }
}


/*
   Function to bring the population through a generation cycle. The top fittest
   words (defined by BOTTLENECK) in the population are randomly crossovered in
   pairs in order to give raise to the progeny. The offspring will substitute
   the bottom words in the population.
*/

void next_generation (int p[P_SIZE][G_SIZE]){
   int fittest[BOTTLENECK];
   int i;
   for (i=0;i<BOTTLENECK;i++){
      fittest[i] = i;
   }

   shuffle_array(fittest, BOTTLENECK);

   for(i=0;i<BOTTLENECK/2;i++){
      crossover(p[fittest[i*2]], p[fittest[(i*2)+1]], p[P_SIZE-i*2-2],
                p[P_SIZE-(i*2)-1], rand()%G_SIZE);
   }
}


/*
   MAIN
   GA simulation
*/

int main(){
   if (P_SIZE % 2 || BOTTLENECK % 2){
      printf("Re-compile with even numbers for P_SIZE and BOTTLENECK in the",
              "#declares\n");
      return 1;
   }

   /* Initialization */
   int P[P_SIZE][G_SIZE];

   rand_population(P);
   print_population(P);
   printf("--------------------------------\n");
   sort_P_by_fitness(P);

   /* Run GA (Selection + Reproduction + Termination) */

   int g=1;
   while (fitness(P[0]) != 1 && g <= MAX_GENERATIONS){
      printf("Best fitness: %.4f\n",fitness(P[0]));
      next_generation(P);
      sort_P_by_fitness(P);
      g++;
   }

   print_population(P);
   printf("Generations: %i\n",g);
   //printf("--------------------------------\n");
   
   return 0;
}
