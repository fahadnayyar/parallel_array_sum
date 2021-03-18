#include <bits/stdc++.h>
#include <sys/sysinfo.h>
#include "parallel_array_sum.h"


int number_of_threads;
ll input_array_size;
ll * input_array;

void verify (ll total_sum) {
   
   ll actual_sum = ((input_array_size)*(input_array_size-1))/2;
   printf("actual_sum: %lld\n", actual_sum);
   printf("total_sum: %lld\n", total_sum);
   if (total_sum == actual_sum) {
      printf("Sum of array is correct.\n");
   }else {
      printf("Sum of array is incorrect.\n");
   }

}


void read_environment_variables() {

   //* Reading the number_of_threads from the environment variable "THREADS"
   char * threads_ptr = (char *)getenv("THREADS");
	if (threads_ptr!=NULL) {
		number_of_threads = atoi(threads_ptr);
      printf("Executing parallel_array_sum using %d threads.\n\n", number_of_threads);
	}else {
      number_of_threads = get_nprocs();
      printf("Executing parallel_array_sum using %d threads = (DEFAULT number of processors available).\n\n", number_of_threads);
   }
   
   // thread_sum_array_size = pow(2, ceil(log(number_of_threads)/log(2)));
   // #ifdef DEBUG
   //    printf("thread_sum_array_size: %d\n\n", thread_sum_array_size);
   // #endif
   
   // thread_sum_array = (ll *) calloc(thread_sum_array_size, sizeof(ll));

   //* Reading the input_array_size from the environment variable "ARRAY_SIZE"
   char * input_array_size_ptr = (char *)getenv("ARRAY_SIZE");
	if (input_array_size_ptr!=NULL) {
		input_array_size = atoi(input_array_size_ptr);
      printf("Size of array = %lld.\n\n", input_array_size);
	}else {
      input_array_size = 1024*1024;
      printf("Size of array = %lld.\n\n", input_array_size);
   }
   // chunk = input_array_size/number_of_threads;
   // printf("chunk_size: %d\n\n", chunk_size);

}


ll * initialize_array() {

   //* intitializing the array
   input_array = (ll *) malloc(sizeof(ll)*input_array_size);
   for (int index=0; index<input_array_size; index++) {
      input_array[index] = index; 
   }
   return input_array;

}


int main ( void ) {
   
   //* Reading the number_of_threads and input_array_size from environment variables.
   read_environment_variables();

   //* intitializing the array
   initialize_array();

   #ifdef DEBUG
      print_input_array();
   #endif

   struct timespec begin, end; 
   clock_gettime(CLOCK_REALTIME, &begin);

   // ll total_sum = parallel_thread_sum();
   parallel_sum_namespace::ParallelSum parallel_sum (input_array, input_array_size, number_of_threads);
   ll total_sum = parallel_sum.giveParallelSumOfArray();

   clock_gettime(CLOCK_REALTIME, &end);
   long seconds = end.tv_sec - begin.tv_sec;
   long nanoseconds = end.tv_nsec - begin.tv_nsec;
   double elapsed = seconds + nanoseconds*1e-9;
   std::cout  << "Total time taken (millisec): " << elapsed*1000 << std::endl;
   
   verify(total_sum);

   free(input_array);

   return 0;
   
}