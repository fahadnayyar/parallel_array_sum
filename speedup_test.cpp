#include <bits/stdc++.h>
#include <sys/sysinfo.h>
#include "parallel_array_sum.h"

#define DEFAULT_ARRAY_SIZE 1024*1024*1024

ll * generate_array (ll input_array_size) {
  ll * input_array = (ll *) malloc(sizeof(ll)*input_array_size); 
  for (int i=0; i<input_array_size; i++) {
     input_array[i] = i%100000;
  }
  return input_array;
}

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
      // # ifdef DEBUG
         printf("Sum of array is correct.\n");
      // #endif
      return true;
   }else {
      // # ifdef DEBUG   
         printf("Sum of array is incorrect.\n");
      // #endif
      return false;
   }

}


void print_parallel_sum_time (ll input_array_size, ll * input_array, int number_of_threads) {

   struct timespec begin, end; 
   clock_gettime(CLOCK_REALTIME, &begin);

   parallel_sum_namespace::ParallelSum parallel_sum (input_array, input_array_size, number_of_threads);
   ll total_parallel_sum = parallel_sum.giveParallelSumOfArray();

   clock_gettime(CLOCK_REALTIME, &end);
   long seconds = end.tv_sec - begin.tv_sec;
   long nanoseconds = end.tv_nsec - begin.tv_nsec;
   double elapsed = seconds + nanoseconds*1e-9;
   std::cout  << "Total time taken (millisec): " << elapsed*1000 << std::endl;

   verify(total_parallel_sum, input_array, input_array_size);
   
}

int main ( int argc, char *argv[] ) {
   
   char * input_array_size_ptr = argv[1];
   ll input_array_size;
   if (input_array_size_ptr == NULL) {
      input_array_size = DEFAULT_ARRAY_SIZE;
   }else {
      input_array_size = atoi(input_array_size_ptr);
   }
 
   ll * input_array = generate_array(input_array_size);

   for (int threads=1; threads<32; threads++) {
      print_parallel_sum_time (input_array_size, input_array, threads);
   }
   
   free(input_array);

   return 0;
   
}