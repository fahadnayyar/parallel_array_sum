namespace parallel_sum_namespace {
   
   // #define DEBUG
   #define ll long long int

   class ParallelSum {
      
      //* class fields
      //* TODO: add description of class fields and APIs.
      
      ll input_array_size;
      ll * input_array;
      
      int thread_sum_array_size;
      ll * thread_sum_array;
      
      int number_of_threads;
      int chunk_size;
      
      //* provate method declarations.
      ll parallel_thread_sum();
      static void * thread_sum_function (void * thread_number_ptr);
      ll combime_thread_sums();
      static void * strided_addition_function (void * index_ptr);
      
      //* helper methods.
      void print_thread_sum_array();
      void print_input_array();

   public:
      
      //* public method declarations.
      ParallelSum ( ll * input_array_, ll input_array_size_, int number_of_threads_ );
      ll giveParallelSumOfArray();
      ~ParallelSum();

   };


   struct pthreadFunctionArgs_1 {
      int thread_number;
      int chunk_size;
      ll * input_array;
      ll * thread_sum_array;
   };


   struct pthreadFunctionArgs_2 {
      int index;
      int stride;
      ll * thread_sum_array;
   };

}