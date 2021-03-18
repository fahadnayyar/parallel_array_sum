/* Implementation of ParallelSum class, which adds elements of an array using threads.
 * The runtime complexity of parallel addition is: O(M/N + log(N)), M: length of array, and N: number of threads used.
 * NOTE: in the comments N means the number_of_threads used and M denote the size_of_input_array.
 * @author: Fahad Nayyar, 2021
 */


#include <bits/stdc++.h>
#include <stdio.h>
#include <pthread.h>
#include "parallel_array_sum.h"


//* The class ParallelSum and its APIs are defined in parallel_sum_namespace
using namespace parallel_sum_namespace;


/* This is the only constructor of ParallelSum class. It takes three inpts:
 *  1.) input_array_: An array of type long long int.
 *  2.) input_array_size_: The size of the input array.
 *  3.) number_of_threads_: The number of threads to be used for parallel addition of array element.
 */ 
ParallelSum::ParallelSum ( ll * input_array_, ll input_array_size_, int number_of_threads_ ) {
   
   //* Initializing the fields of ParallelSum class accoring to the inputs to constructors.
   input_array = input_array_;
   input_array_size = input_array_size_;
   number_of_threads = number_of_threads_;
   chunk_size = input_array_size / number_of_threads;
   
   /* The thread_sum_array_size is initialized to the next power of 2 greater than number_of_threads
    * to make sure the O(lon N) time to reduce the summation of thread local sum works for any number of threads.   
   */
   thread_sum_array_size = pow(2, ceil(log(number_of_threads)/log(2)));
   thread_sum_array = (ll *) calloc(thread_sum_array_size, sizeof(ll));

   //* Debugging messages.
   #ifdef DEBUG
      printf("ParallelSum constructor called with parameters: input_array_size_: %lld, number_of_threads_= %d\n", input_array_size_, number_of_threads_);
      printf("input_array_size: %lld\n", input_array_size);
      printf("number_of_threads: %d\n", number_of_threads);
      printf("thread_sum_array_size: %d\n", thread_sum_array_size);
      printf("chunk_size: %d\n\n\n", chunk_size);
   #endif

}


/* Only public method accessable to users other than constructor.
 * This retuns the final summation of elements of input_array.
*/
ll ParallelSum::giveParallelSumOfArray() {
   
   return parallel_thread_sum();

}


/* This private method executes the whole parallel sum algorithm in the folowing steps:
   0.) It implicitly partitions the input_array in N chunks of size M/N. (N+1 chunks in case of M%N != 0).
   1.) It swaps N (number_of_threads) threads to do dum of M/N sized chunks of the input_array
      Each thread os thread_number I writes the sum of their chunks into thread_sum_array[I]. : O(M/N)
   2.) It calls combime_thread_sums which combines the N elements of thread_sum_array in O(Log N) time.: O(Log N)
   3.) For the case when M%N != 0, this method calculates the the sum of lase left_over_chunk sequentially
      The left_over chunk is less than of size M/N so this sequential step doesn't affect the runtime complexity: O(M/N) time
   4.) It adds the sum of left_over chunk and reult of combime_thread_sums to get the total_parallel_sum.
*/
ll ParallelSum::parallel_thread_sum() {
   
   pthread_t thread_array[number_of_threads]; 
   
   //* Spawing number_of_threads threads to do additions of input_array_size/number_of_threads size chunks.
   for (int i=0; i<number_of_threads ; i++) {
      struct pthreadFunctionArgs_1 * pthreadFunctionArgs_1_ptr = (struct pthreadFunctionArgs_1 * ) malloc(sizeof(pthreadFunctionArgs_1));
      pthreadFunctionArgs_1_ptr->thread_number = i;
      pthreadFunctionArgs_1_ptr->chunk_size = chunk_size;
      pthreadFunctionArgs_1_ptr->input_array = input_array;
      pthreadFunctionArgs_1_ptr->thread_sum_array = thread_sum_array;
      pthread_create(&thread_array[i], NULL, &ParallelSum::thread_sum_function, pthreadFunctionArgs_1_ptr);
   }

   //* waiting all the threads to complete adding their chunks
   for (int i = 0; i < number_of_threads; i++) {
      pthread_join(thread_array[i], NULL);
   }

   //* sum of leftover array
   ll left_over_sum = 0;
   int left_over_start = (chunk_size*number_of_threads);
   for (int i=left_over_start; i<input_array_size; i++){
      left_over_sum += input_array[i];
   }
   
   //* Debugging messages
   #ifdef DEBUG 
      printf("left_over_start: %d", left_over_start);
      printf("left_over_sum: %lld\n\n", left_over_sum); 
      print_thread_sum_array();
   #endif

   
   /* O(Log N) operation to combine the results of addition done by each thread.
      This step does all modifications in the thread_sum_array (in place)
   */
   ll combined_sum = combime_thread_sums();
   
   //* Debugging message.
   #ifdef DEBUG
      print_thread_sum_array();
   #endif
   
   //* COmbining left_over chunk sum and sum of resutls of individual threads to get the final total_sum
   ll total_sum = combined_sum + left_over_sum;
   
   return total_sum;

}


/* This routine is passed to pthread_create. 
 * Each thread adds their respective chunk of size M/N: So runtime complexity is O(M/N).
 * Thread I does addition for input_array from (I * chunk_size) till (I * chunk_size + chunk_size). 
 * The arg is used to access the thread_number (or thread id), chunk_size, input_array, thread_sun_array. 
 * The result of addition by thread I is written to thread_sum_array[I].
 * Since each thread handles different chunk of input_array and writes outpu to different index in thread_sun_array, 
 *  there is no concurrent access and no race conditions.
 */
void * ParallelSum::thread_sum_function (void * arg) {
   
   struct pthreadFunctionArgs_1 * pthreadFunctionArgs_1_ptr = (struct pthreadFunctionArgs_1 *) arg;
   int thread_number = pthreadFunctionArgs_1_ptr->thread_number;
   int chunk_size_ = pthreadFunctionArgs_1_ptr->chunk_size;
   ll * input_array_ = pthreadFunctionArgs_1_ptr->input_array;
   ll * thread_sum_array_ = pthreadFunctionArgs_1_ptr->thread_sum_array;

   int start = thread_number * chunk_size_;
   int end = start + chunk_size_;

   #ifdef DEBUG
      printf("start: %d, end: %d\n\n", start, end);
   #endif
   
   ll thread_sum = 0;
   for ( int i = start; i < end; i++ ) {
      thread_sum += input_array_[i];
   }
   thread_sum_array_[thread_number] = thread_sum;
   free(arg);

}


/* This routime adds the result of each threa'd sum stored in thread_sum_array in O(Log N) time.
 * It does parallel and in_place addition of thread_sum_array.
 * It starts with stride = 1 wich is doubled until stride < N.
 * It does stided addition with number of threads decreasing by a factor of 2 with each doubling of sttide.
 * Example: [1, 2, 3, 4, 5, 6, 7, 8]
   After parallel addition using Stride 1:  [1, 1+2, 3, 3+4, 5, 5+6, 7, 7+8] (using N/2 = 4 threads)
   After parallel addition using Stride 2:  [1, 1+2, 3, 1+2+3+4, 5, 5+6, 7, 5+6+7+8] (using N/4 = 2 threads)
   After parallel addition using Stride 4:  [1, 1+2, 3, 1+2+3+4, 5, 5+6, 7, 1+2+3+4+5+6+7+8] (using N/8 = 1 thread)
   So the result of summation if stored in the last index of the thread_sum_array.
 * If N is not a power of 2, then also thread_sum_array was initialized to next power of 2 greater than N.
   The adiditonal indexes were initialized to 0, thus thie extra part of thread_sum_array does not affct the sum.
 */
ll ParallelSum::combime_thread_sums() {

   pthread_t thread_array[thread_sum_array_size]; 
   int stride = 1;
   std::vector<int> threads_spawned;
   while (stride < thread_sum_array_size) {
      for (int i = 0; i < thread_sum_array_size; i++) {
         int thread_number = i;
         int index = (thread_number+1)*stride*2 - 1;
         if (index < thread_sum_array_size) {
            struct pthreadFunctionArgs_2 * pthreadFunctionArgs_2_ptr = (struct pthreadFunctionArgs_2 * ) malloc(sizeof(pthreadFunctionArgs_2));
            pthreadFunctionArgs_2_ptr->index = index;
            pthreadFunctionArgs_2_ptr->stride = stride;
            pthreadFunctionArgs_2_ptr->thread_sum_array = thread_sum_array;
            pthread_create(&thread_array[i], NULL, &ParallelSum::strided_addition_function, pthreadFunctionArgs_2_ptr);
            threads_spawned.push_back(i);
         }
      }
      for (auto& iterator : threads_spawned) { 
        pthread_join(thread_array[iterator], NULL);
      }
      threads_spawned.clear(); 
      stride *= 2;
   }
   return thread_sum_array[thread_sum_array_size-1];

}


/* This routime is passed to pthread_create in combime_thread_sums. 
 * It gets the index, stride and combime_thread_sums through the arg passed.
 * It just adds (index) and (index-stride) elements of thread_sum_array and stored the result in thread_sum_array[index]
 * Note: for stride = S, (N/2*S) threads execute this routime.
 * Runtime complexity of this routime: O(1)
 */
void *  ParallelSum::strided_addition_function (void * arg) {
   
   struct pthreadFunctionArgs_2 * pthreadFunctionArgs_2_ptr = (struct pthreadFunctionArgs_2 *) arg;
   int index = pthreadFunctionArgs_2_ptr->index;
   int stride = pthreadFunctionArgs_2_ptr->stride;
   ll * combime_thread_sums_ = pthreadFunctionArgs_2_ptr->thread_sum_array;
   thread_sum_array_[index] += thread_sum_array_[index-stride];
   free(arg);

}


//* Helper debugging routime to print all the elements of thread_sum_array
void ParallelSum::print_thread_sum_array() {

   for (int i=0; i<thread_sum_array_size; i++) {
      printf("%lld, ", thread_sum_array[i]);
   }
   printf("\n\n");

}


//* Helper debugging routime to print all the elements of input_array
void ParallelSum::print_input_array() {
   
   for (int i=0; i < input_array_size; i++) {
      printf("%lld, ", input_array[i]);
   }
   printf("\n\n");

}


/* Destructor routine frees the thread_sum_array, which is the only major 
 * data structure allocated by class ParallelSum.
*/  
ParallelSum::~ParallelSum() { 
   free(thread_sum_array); 
}