/* Declatiaon of ParallelSum class and its APIs. 
 * This calss does parallel addition of elements of input_array using threads.
 * The runtime complexity of parallel addition is: O(M/N + log(N)), M: length of array, and N: number of threads used.
 * NOTE: in the comments N means the number_of_threads used and M denote the size_of_input_array.
 * NOTE: ll is defined as lng long int
 * @author: Fahad Nayyar, 2021
 */


namespace parallel_sum_namespace {
   
   // #define DEBUG
   #define ll long long int

   class ParallelSum {
      
      //* class fields

      //* size of the input_array
      ll input_array_size;
      //* input_array is the array whose sum is to be computed
      ll * input_array;
      
      //* size of the thread_sum_array_size
      int thread_sum_array_size;
      //* thread_sum_array_size stores the sum of chunks of input_array calculated by each
      ll * thread_sum_array;
      
      int number_of_threads;
      int chunk_size;
      
      //* private method declarations.
      
      //* Divides input array into N chunks of size M/N. Each chunk is added by a different thread: O(M/N)
      ll parallel_thread_sum();
      //* combines the result of parallel addition of chunks to get the total_sum: O(Log N)
      ll combime_thread_sums();
      
      //* function used by pthreads
      static void * thread_sum_function (void * thread_number_ptr);
      static void * strided_addition_function (void * index_ptr);
      
      //* helper methods.
      void print_thread_sum_array();
      void print_input_array();

   public:
      
      //* public method declarations.
      
      //* Constructor.
      ParallelSum ( ll * input_array_, ll input_array_size_, int number_of_threads_ );
      
      //* The major API of ParallelSum class which resturns the sum of elements of input_array.
      ll giveParallelSumOfArray();
      
      ~ParallelSum();

   };


   //* This sturct is used to pass arguments to pthread function thread_sum_function
   struct pthreadFunctionArgs_1 {
      int thread_number;
      int chunk_size;
      ll * input_array;
      ll * thread_sum_array;
   };


   //* This sturct is used to pass arguments to pthread function strided_addition_function
   struct pthreadFunctionArgs_2 {
      int index;
      int stride;
      ll * thread_sum_array;
   };

}