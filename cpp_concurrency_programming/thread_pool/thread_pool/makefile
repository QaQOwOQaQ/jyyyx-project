FLAGS = -Wall -Wextra -Iinclude/

CAPP = bin/thread_pool
CPPAPP = bin/cpp_thread_pool

CSRC = src/thread_pool.c src/main.c
CPPSRC = src/cpp_main.cpp


.PHONY:cpp
cpp: 
	clear 
	clear 
	g++ $(FLAGS) $(CPPSRC) -o $(CPPAPP)
	./$(CPPAPP)


.PHONY:c
c: 
	clear 
	clear 
	gcc $(FLAGS) -pthread $(CSRC) -o $(CAPP)
	./$(CAPP)

.PHONY:clean
clean:
	rm -rf bin/*