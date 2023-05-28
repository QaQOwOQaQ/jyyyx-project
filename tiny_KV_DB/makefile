FLAGS = -Wall -Wextra -Iinclude/

.PHONY:app
app:
	clear 
	g++ $(FLAGS) main.cpp -o bin/app
	./bin/app

stress-test:
	g++ $(FLAGS) stress_test/stress_test.cpp -o bin/stress-test
	./bin/stress-test

.PHONY:clean
clean:
	rm bin/*
