FLAGS = -Isrc/ -Og -Werror -Wextra -Wall
NOWERROR = -Isrc/ -Og -Wextra -Werror
RUBBSH = *.o *.i a.out bin/*

.PHONY: run
app:
	clear 
	clear 
	g++ src/main.cpp $(FLAGS) -o bin/app 
	./bin/app

.PHONY: debug
debug:
	clear 
	clear 
	echo "make no wall!"
	g++ src/main.cpp $(NOWERROR) -o bin/app 
	./bin/app


.PHONY: clean
clean:
	rm -rf $(RUBBSH)

