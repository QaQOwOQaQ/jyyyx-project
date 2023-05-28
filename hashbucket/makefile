CC = gcc
FLAGS = -Wall -Werror -I.
RENAME = app
ADD = hash_table.c
APP = main.c
DEBUG = -DDEBUG

.PHONY:app
app: main.c
	$(CC) $(FLAGS) -o $(RENAME) $(ADD) $(APP)
	./$(RENAME)

.PHONY:debug
debug:main.c
	$(CC) $(FLAGS) $(DEBUG) -o $(RENAME) $(ADD) $(APP)
	./$(RENAME)

.PHONY:clean
clean:
	rm -rf *.o *.i a.out