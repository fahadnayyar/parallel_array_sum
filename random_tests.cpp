/* Script to test the correctness of parallel addition using ParallelSum class. 
 * This script generates random arrays of random sizes and 
   add them using a varaiety of number of threads. 
 * This is to check that ParallleSum can handle input_array of any size and any possible number_of_threads. 
 * @author: Fahad Nayyar, 2021
 */


#include <bits/stdc++.h>
#include <sys/sysinfo.h>
#include "parallel_array_sum.h"
#include <stdlib.h>
#include <time.h>


using namespace std;


//* Routime to verify that the reuslt from parallle addition is same as actual sum of input_array
bool verify ( ll parallel_sum_total, ll* input_array, ll input_array_size ) {
   
   ll actual_sum = 0;
   for (int i=0; i<input_array_size; i++) {
      actual_sum += input_array[i];
   }
   # ifdef DEBUG
      printf("actual_sum: %lld\n", actual_sum);
      printf("parallel_sum_total: %lld\n", parallel_sum_total);
   #endif
   if (parallel_sum_total == actual_sum) {
      # ifdef DEBUG
         printf("Sum of array is correct.\n");
      #endif
      return true;
   }else {
      // # ifdef DEBUG   
         printf("Sum of array is incorrect.\n");
      // #endif
      return false;
   }

}


//* Procedure to generate random integer in range 0, 100000
int generate_random_integer () {
   srand(time(0));
   return rand()%100000;
}


//* Generating ranom array of size input_array_size.
ll * generate_random_array (ll input_array_size) {
  ll * input_array = (ll *) malloc(sizeof(ll)*input_array_size); 
  for (int i=0; i<input_array_size; i++) {
     input_array[i] = generate_random_integer();
  }
  return input_array;
}


int main ( int argc, char *argv[] ) {
   
   int num_tests = 1000;
   bool tests_passed = true;
   
   //* Running 1000 random test cases
   for ( int i=1; i <= num_tests; i++ ) {
      ll input_array_size = generate_random_integer();
      ll * input_array = generate_random_array(input_array_size);
      int number_of_threads = i;
      parallel_sum_namespace::ParallelSum parallel_sum (input_array, input_array_size, number_of_threads);
      ll parallel_sum_total = parallel_sum.giveParallelSumOfArray();
      tests_passed &= verify(parallel_sum_total, input_array, input_array_size);
      free(input_array);
   }

   //* Displaying the result of tests into the console
   if (tests_passed){
      printf("All random tests passed\n");
   }else {
      printf("Some random test failed\n");
   }

   return 0;
   
}