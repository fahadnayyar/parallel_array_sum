# parallel_array_sum
A library to do parallel addition of array using pthreads in O(M/N + Log N) time. (M = size of input array, N = number of threads used for addition)/

## To build:
$ make
 
## To run parallel_speedup_test: 
$ ./speedup_test

## To run random_tests:
$ ./random_tests

## To use the paralll_sum interactively:
$./interactive <NUMBER_OF_THREADS> <SIZE_OF_INPUT_ARRAY>
$ entry the elements of the array


## To delete the executable:
$ make clean