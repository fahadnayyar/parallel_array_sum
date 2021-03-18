default: speedup_test random_tests interactive

parallel_array_sum.o: parallel_array_sum.cpp parallel_array_sum.h
	g++ -g parallel_array_sum.cpp -c -o parallel_array_sum.o -lpthread

random_tests: parallel_array_sum.o random_tests.cpp
	g++ -g random_tests.cpp parallel_array_sum.o -o random_tests -lpthread

interactive: parallel_array_sum.o interactive.cpp
	g++ -g interactive.cpp parallel_array_sum.o -o interactive -lpthread

speedup_test: parallel_array_sum.o speedup_test.cpp
	g++ -g speedup_test.cpp parallel_array_sum.o -o speedup_test -lpthread

run_random_tests:
	./random_tests

run_speedup_test:
	./speedup_test

clean:
	rm random_tests
	rm speedup_test
	rm interactive
	rm *.o