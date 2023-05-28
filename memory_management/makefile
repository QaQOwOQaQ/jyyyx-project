CC = g++
FLAGS = -Wall -Wextra 
TEST = src/test.cpp
TEST_BIN = bin/test

ALLOCA1 = src/allocator/allocator1.cpp
ALLOCA1_BIN = bin/allocator1

ALLOCA2 = src/allocator/allocator2.cpp
ALLOCA2_BIN = bin/allocator2

ALLOCA3 = src/allocator/allocator3.cpp
ALLOCA3_BIN = bin/allocator3

ALLOCA4 = src/allocator/allocator4.cpp
ALLOCA4_BIN = bin/allocator4

MALLOC = src/malloc/mem_malloc.c
MALLOC_BIN = bin/mem_malloc




# ---------------------------------------------------------------------------------
.POHONY:malloc
malloc:
	$(CC) $(FLAGS) $(MALLOC) -o $(MALLOC_BIN)
	./$(MALLOC_BIN)


.POHONY:a4
a4:
	$(CC) $(FLAGS) $(ALLOCA4) -o $(ALLOCA4_BIN)
	./$(ALLOCA4_BIN)

.POHONY:a3
a3:
	$(CC) $(FLAGS) $(ALLOCA3) -o $(ALLOCA3_BIN)
	./$(ALLOCA3_BIN)


.POHONY:a2
a2:
	$(CC) $(FLAGS) $(ALLOCA2) -o $(ALLOCA2_BIN)
	./$(ALLOCA2_BIN)


.POHONY:a1
a1:
	$(CC) $(FLAGS) $(ALLOCA1) -o $(ALLOCA1_BIN)
	./$(ALLOCA1_BIN)


.POHONY:test
test:
	$(CC) $(FLAGS) $(TEST) -o $(TEST_BIN)
	./$(TEST_BIN)


.POHONY:clean
clean:
	rm -rf *.o *.i *.out *.in bin/*