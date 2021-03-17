#include <bits/stdc++.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <pthread.h> 

// #define DEBUG
#define ll long long int

int number_of_threads;
ll size_of_array;
ll * input_array;
ll * thread_sum_array;
int chunk;
int stride;

using namespace std;

void read_environment_variables();
void initialize_array();
ll parallel_thread_sum();
void * thread_sum_function (void * thread_number_ptr);
ll combime_thread_sums();
void * strided_addition_function (void * index_ptr);
void verify (ll total_sum);

void print_thread_sum_array() {
   
   for (int i=0; i<number_of_threads; i++) {
      printf("%lld, ", thread_sum_array[i]);
   }
   printf("\n\n");

}

void print_input_array() {
   
   for (int i=0; i<size_of_array; i++) {
      printf("%lld, ", input_array[i]);
   }
   printf("\n\n");

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
   thread_sum_array = (ll *) malloc(sizeof(ll)*number_of_threads);

   //* Reading the size_of_array from the environment variable "ARRAY_SIZE"
   char * size_of_array_ptr = (char *)getenv("ARRAY_SIZE");
	if (size_of_array_ptr!=NULL) {
		size_of_array = atoi(size_of_array_ptr);
      printf("Size of array = %lld.\n\n", size_of_array);
	}else {
      size_of_array = 1024*1024;
      printf("Size of array = %lld.\n\n", size_of_array);
   }
   chunk = size_of_array/number_of_threads;
   printf("chunk: %d\n\n", chunk);

}

void initialize_array() {

   //* intitializing the array
   input_array = (ll *) malloc(sizeof(ll)*size_of_array);
   for (int index=0; index<size_of_array; index++) {
      input_array[index] = index; 
   }

}

ll parallel_thread_sum() {
   
   pthread_t thread_array[number_of_threads]; 
   
   //* Spawing number_of_threads threads to do additions of size_of_array/number_of_threads size chunks.
   for (int i=0; i<number_of_threads ; i++) {
      int * thread_number = (int *) malloc(sizeof(int));
      *thread_number = i;
      pthread_create(&thread_array[i], NULL, thread_sum_function, thread_number);
   }

   //* waiting all the threads to complete adding their chunks
   for (int i = 0; i < number_of_threads; i++) {
      pthread_join(thread_array[i], NULL);
   }

   //* sum of leftover array
   ll left_over_sum = 0;
   int left_over_start = (chunk*number_of_threads);
   for (int i=left_over_start; i<size_of_array ;i++){
      left_over_sum += input_array[i];
   }
   
   #ifdef DEBUG 
      printf("left_over_start: %d", left_over_start);
      printf("left_over_sum: %lld\n\n", left_over_sum); 
   #endif

   #ifdef DEBUG
      print_thread_sum_array();
   #endif
   
   ll combined_sum = combime_thread_sums();
   
   #ifdef DEBUG
      print_thread_sum_array();
   #endif
   
   ll total_sum = combined_sum + left_over_sum;
   
   return total_sum;


}

void * thread_sum_function (void * thread_number_ptr) {
   
   int thread_number = *((int *) thread_number_ptr);
   int start = thread_number*chunk;
   int end = start + chunk;

   #ifdef DEBUG
      printf("start: %d, end: %d\n\n", start, end);
   #endif
   
   ll thread_sum = 0;
   for ( int i = start; i < end; i++ ) {
      thread_sum += input_array[i];
   }
   thread_sum_array[thread_number] = thread_sum;
   free(thread_number_ptr);

}

ll combime_thread_sums() {

   pthread_t thread_array[number_of_threads]; 
   stride = 1;
   vector<int> threads_spawned;
   while (stride < number_of_threads) {
      for (int i=0; i<number_of_threads ; i++) {
         int thread_number = i;
         int * index = (int *) malloc(sizeof(int));
         *index = (thread_number+1)*stride*2 - 1;
         if (*index < number_of_threads) {
            pthread_create(&thread_array[thread_number], NULL, strided_addition_function, index);
            threads_spawned.push_back(i);
         }
      }
      for (auto& iterator : threads_spawned) { 
        pthread_join(thread_array[iterator], NULL);
      } 
      stride *= 2;
   }
   return thread_sum_array[number_of_threads-1];

   // ll total_sum = 0;
   // //* adding all the sums from threads
   // for (int i=0; i<number_of_threads; i++) {
   //    total_sum += thread_sum_array[i];
   // }
   // return total_sum;

}

void * strided_addition_function (void * index_ptr) {
   
   int index = *((int *)index_ptr);
   thread_sum_array[index] += thread_sum_array[index-stride];
   free(index_ptr);

}

void verify (ll total_sum) {
   
   ll actual_sum = ((size_of_array)*(size_of_array-1))/2;
   printf("actual_sum: %lld\n", actual_sum);
   printf("total_sum: %lld\n", total_sum);
   if (total_sum == actual_sum) {
      printf("Sum of array is correct.\n");
   }else {
      printf("Sum of array is incorrect.\n");
   }

}

int main ( void ) {
   
   //* Reading the number_of_threads and size_of_array from environment variables.
   read_environment_variables();

   //* intitializing the array
   initialize_array();
   
   #ifdef DEBUG
      print_input_array();
   #endif

   struct timespec begin, end; 
   clock_gettime(CLOCK_REALTIME, &begin);

   ll total_sum = parallel_thread_sum();

   clock_gettime(CLOCK_REALTIME, &end);
   long seconds = end.tv_sec - begin.tv_sec;
   long nanoseconds = end.tv_nsec - begin.tv_nsec;
   double elapsed = seconds + nanoseconds*1e-9;
   std::cout  << "Total time taken (millisec): " << elapsed*1000 << std::endl;
   
   verify(total_sum);

   free(input_array);
   free(thread_sum_array);

   return 0;
   
}

// S -> size of array
// N -> numbe of threads
// O(S/N) + O(N)

// O(S/N) + O(log(n))

// [10,2,7,4]

// arr[N]
// N
// N -> N/2 -> N/4 ... 1 