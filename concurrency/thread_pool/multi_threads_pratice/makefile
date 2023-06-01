CC = gcc
FLAGS = -Wall -lpthread # libpthread.so
APP = thread
TEST = test


.PHONY:app
app: 
	clear 
	$(CC) thread.c $(FLAGS) -I. -o $(APP)
	./$(APP)

.PHONY:test
test: 
	clear 
	$(CC) test.c $(FLAGS) -I. -o $(TEST)
	./$(TEST)


.PHONY:clean
clean:
	rm -rf *.o *.i *.out $(APP) $(TEST)