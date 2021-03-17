default: parallel_array_sum 

parallel_array_sum: parallel_array_sum.cpp
	g++ -g parallel_array_sum.cpp -o parallel_array_sum -lpthread

run:
	./parallel_array_sum

clean:
	rm ./parallel_array_sum