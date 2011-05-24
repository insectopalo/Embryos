/*
   Very simple genetic algorithm implementation for a toy model. The organisms
   are n-bits computer words. The fitness function is the number of ones in the
   word.
   The population is produced by initializing m words randomly. Then
   iteratively evaluate the fitness of each vector, keep the b vectors with the
   highest values of it and apply 1-point corssover to these, producing b new
   words -the parents are chosen randomly, there's no competition-. The new b
   words substitute the bottom b words in the fitness-sorted population.
   Break the loop when bmax generations have been run or when a word reaches a
   fitness of 1.

   Author Gonzalo S Nido <insectopalo@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>

/* 
   Definition of the constants used in the simulation.
   - GENOME_LENGTH (n) is the length in bits of each computer word.
   - POPULATION_SIZE (m) is the number of words in the population
   - BOTTLENECK (b) is the number of genomes that survive each generation giving
     rise to the progeny by crossover
   - MAX_GENERATIONS (bmax) is the maximum number of generations in the simulation 
*/
#define GENOME_LENGTH 16
#define POPULATION_SIZE 40
#define BOTTLENECK 20
#define MAX_GENERATIONS 10

/*
   Type definitions for the "organism" (or word), a simple int vector of size
   GENOME_LENGTH and "population", a vector of pointers to "organism" data
   types
*/

typedef int ORGANISM[GENOME_LENGTH];

typedef struct population {
   ORGANISM member[POPULATION_SIZE];
} POPULATION;


/*
   Function to generate a random population. The values in the ORGANISM type
   vector are binary (rand() % 2). Note that the POPULATION data type must
   be initialized beforehand, and the argument given to the function is the
   pointer
*/

void rand_population(POPULATION *P, int pop_size, int gen_size) {
   srand( (unsigned int) time( NULL ));
   int i=0;
   for (i=0;i<pop_size;i++){
      int j=0;
      for (j=0;j<gen_size;j++){
         (*P).member[i][j] = rand() % 2;
      }
   }
}


/*
   Function to calculate the fitness (between 0 and 1) of a binary vector. The
   pointer to the binary vector is given as an argument (note that is a
   ORGANISM data type, since it is of constant length)
*/

float fitness(ORGANISM * Org){
   float f=0;
   int j=0;
   for (j=0;j<GENOME_LENGTH;j++){
      f += (*Org)[j];
   }
   f/=GENOME_LENGTH;
   return f;
}


/*
   Function to print the population, one genome per line, with its associated
   fitness
*/

void print_population ( POPULATION * P, int pop_size, int gen_size){
   int i=0;
   for (i=0;i<pop_size;i++){
      int j=0;
      for (j=0;j<gen_size;j++){
         printf ("%i",(*P).member[i][j]);
      }
      printf (" f=%.4f\n",fitness(&(*P).member[i]));
   }
}


/*
   Function to do a crossover between two parents generating two descendants.
   All organism vectors must be declared beforehand and their pointers given
   as arguments. A last argument gives the point where the crossover happens
*/

void crossover(ORGANISM * parent1, ORGANISM * parent2,
               ORGANISM * offspring1, ORGANISM * offspring2,
               int cutting_point){
   int j;
   for (j=0;j<cutting_point;j++){
      (*offspring1)[j] = (*parent1)[j];
      (*offspring2)[j] = (*parent2)[j];
   }
   for (j=cutting_point;j<GENOME_LENGTH;j++){
      (*offspring1)[j] = (*parent2)[j];
      (*offspring2)[j] = (*parent1)[j];
   }
}


/*
   Functions to sort the organisms in the population according to their
   respective fitness. The function "comp" is used internally in
   sort_P_by_fitness, which is an implementation of the Quicksort
   algorithm
*/   

int comp(const void *a, const void *b){
   float diff = fitness((ORGANISM *)a) - fitness((ORGANISM *)b);
   if (diff == 0){
      return 0;
   }else if (diff < 0){
      return 1;
   }else{
      return -1;
   }
}

void sort_P_by_fitness(POPULATION * P, int pop_size){
   qsort((*P).member, pop_size, sizeof(ORGANISM), comp);
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
   Function to bring the POPULATION through a generation. It randomly shuffles
   the top BOTTLENECK organisms and mates them in order to give raise to the
   offspring. Each pair of top fitness organisms give raise to a pair of
   descendants wich substitute the bottom BOTTLENECK organisms in the fitness-
   sorted POPULATION. 
*/

void next_generation (POPULATION * P){
   int sex_candidates[BOTTLENECK];
   int i;
   for (i=0;i<BOTTLENECK;i++){
      sex_candidates[i] = i;
   }

   shuffle_array(sex_candidates, BOTTLENECK);

   for(i=0;i<BOTTLENECK/2;i++){
      crossover(&(*P).member[sex_candidates[i*2]], &(*P).member[sex_candidates[(i*2)+1]],
                &(*P).member[POPULATION_SIZE - i*2 -2], &(*P).member[POPULATION_SIZE - (i*2) - 1],
                rand() % GENOME_LENGTH);
   }
}

int main(){
   if (POPULATION_SIZE % 2 || BOTTLENECK % 2){
      printf("Re-compile with even numbers for POPULATION_SIZE and BOTTLENECK in the #declares\n");
      return 1;
   }

   /* Initialization */

   POPULATION P;
   rand_population(&P,POPULATION_SIZE,GENOME_LENGTH);
   sort_P_by_fitness(&P,POPULATION_SIZE);
   //print_population(&P,POPULATION_SIZE,GENOME_LENGTH);

   /* Run GA (Selection + Reproduction + Termination) */

   int g=1;
   while (fitness(P.member) != 1 && g <= MAX_GENERATIONS){
      printf("Best fitness: %.4f\n",fitness(P.member));
      next_generation(&P);
      sort_P_by_fitness(&P,POPULATION_SIZE);
      g++;
   }

   print_population(&P,POPULATION_SIZE,GENOME_LENGTH);
   printf("Generations: %i\n",g);
   //printf("--------------------------------\n");
   
   return 0;
}
