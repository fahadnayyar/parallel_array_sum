#include <bits/stdc++.h>
#include <stdio.h>
#include <pthread.h>
#include "parallel_array_sum.h"


// using namespace std;

using namespace parallel_sum_namespace;

ParallelSum::ParallelSum ( ll * input_array_, ll input_array_size_, int number_of_threads_ ) {
   
   input_array = input_array_;
   input_array_size = input_array_size_;
   number_of_threads = number_of_threads_;
   chunk_size = input_array_size / number_of_threads;
   thread_sum_array_size = pow(2, ceil(log(number_of_threads)/log(2)));
   thread_sum_array = (ll *) calloc(thread_sum_array_size, sizeof(ll));

   #ifdef DEBUG
      printf("ParallelSum constructor called with parameters: input_array_size_: %lld, number_of_threads_= %d\n", input_array_size_, number_of_threads_);
      printf("input_array_size: %lld\n", input_array_size);
      printf("number_of_threads: %d\n", number_of_threads);
      printf("thread_sum_array_size: %d\n", thread_sum_array_size);
      printf("chunk_size: %d\n\n\n", chunk_size);
   #endif

}


ll ParallelSum::giveParallelSumOfArray() {
   
   return parallel_thread_sum();

}


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
   free(arg); // DOUBT

}


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


void *  ParallelSum::strided_addition_function (void * arg) {
   
   struct pthreadFunctionArgs_2 * pthreadFunctionArgs_2_ptr = (struct pthreadFunctionArgs_2 *) arg;
   int index = pthreadFunctionArgs_2_ptr->index;
   int stride = pthreadFunctionArgs_2_ptr->stride;
   ll * thread_sum_array_ = pthreadFunctionArgs_2_ptr->thread_sum_array;
   thread_sum_array_[index] += thread_sum_array_[index-stride];
   free(arg); // DOUBT

}


void ParallelSum::print_thread_sum_array() {

   for (int i=0; i<thread_sum_array_size; i++) {
      printf("%lld, ", thread_sum_array[i]);
   }
   printf("\n\n");

}

void ParallelSum::print_input_array() {
   
   for (int i=0; i < input_array_size; i++) {
      printf("%lld, ", input_array[i]);
   }
   printf("\n\n");

}

ParallelSum::~ParallelSum() { 
   free(thread_sum_array); 
}