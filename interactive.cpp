/* Script to enable interactive usage of the parallel addition using ParallelSum class. 
 * USAGE: ./interactive <NUMBER_OF_THREADS> <SIZE_OF_INPUT_ARRAY>
   Then input the elements of the input_array.
 * NOTE: ll is defined as lng long int in "parallel_array_sum.h"
 * @author: Fahad Nayyar, 2021
 */

#include <bits/stdc++.h>
#include <sys/sysinfo.h>
#include "parallel_array_sum.h"

using namespace std;

int main ( int argc, char *argv[] ) {
   
   if (argc < 3) {
      cout<<"Usage: " << argv[0] << " <NUMBER_OF_THREADS> <SIZE_OF_INPUT_ARRAY>\n";
      return 1;
   }

   //* Reading NUMBER_OF_THREADS, SIZE_OF_INPUT_ARRAY from command line arguments.
   char * number_of_threads_ptr = argv[1];
   int number_of_threads = atoi(number_of_threads_ptr);
   char * input_array_size_ptr = argv[2];
   ll input_array_size = atoi(input_array_size_ptr);

   printf("Executing parallel_array_sum using %d threads.\n\n", number_of_threads);
   printf("please enter %lld elements of the input_array: \n", input_array_size);

   //* Reading the input array.
   ll * input_array = (ll *) malloc(sizeof(ll)*input_array_size);
   for (int i=0; i<input_array_size; i++) {
      cin >> input_array[i];
   }
   
   //* timing code
   struct timespec begin, end; 
   clock_gettime(CLOCK_REALTIME, &begin);

      //* Doing the parallel addition of input_array
      parallel_sum_namespace::ParallelSum parallel_sum (input_array, input_array_size, number_of_threads);
      ll parallel_total_sum = parallel_sum.giveParallelSumOfArray();
   
   //* timing code
   clock_gettime(CLOCK_REALTIME, &end);
   long seconds = end.tv_sec - begin.tv_sec;
   long nanoseconds = end.tv_nsec - begin.tv_nsec;
   double elapsed = seconds + nanoseconds*1e-9;
   
   //* Displaying the result of addition and time taken on the console.
   cout << "paralle_total_sum = " << parallel_total_sum << endl;
   std::cout  << "Total time taken to add (millisec): " << elapsed*1000 << std::endl;
   
   free(input_array);

   return 0;
   
}