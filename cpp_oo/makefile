CC = g++
FLAGS = -Og -Wall -Werror -DDEBUG

MAIN_COMPLEX = ./complex/complex_test.cpp
BIN_COMPLEX = ./bin/complex_test  # 别脑残写成了/bin，会解释为绝对路径

MAIN_STRING = ./string/string_test.cpp
BIN_STRING = ./bin/string_test  

MAIN_OOP = ./oop/oop_test.cpp
BIN_OOP = ./bin/oop_test  		


.PHONY: app1
app1:
	$(CC) $(FLAGS) $(MAIN_COMPLEX) -o $(BIN_COMPLEX)
	./$(BIN_COMPLEX)

.PHONY: app2
app2:
	$(CC) $(FLAGS) $(MAIN_STRING) -o $(BIN_STRING)
	./$(BIN_STRING)


.PHONY: app3
app3:
	$(CC) $(FLAGS) $(MAIN_OOP) -o $(BIN_OOP)
	./$(BIN_OOP)


.PHONY: clean
clean:
	rm a.out *.o *.i ./bin/*