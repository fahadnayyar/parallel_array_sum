default: test 

test: parallel_array_sum.o test.cpp
	g++ -g test.cpp parallel_array_sum.o -o test -lpthread

parallel_array_sum.o: parallel_array_sum.cpp parallel_array_sum.h
	g++ -g parallel_array_sum.cpp -c -o parallel_array_sum.o -lpthread

run:
	./test

clean:
	rm ./test
	rm *.o