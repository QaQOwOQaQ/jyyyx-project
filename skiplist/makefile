FLAGS = -Wall -Wextra -Werror -Iheader
# 这里的 I 解释是基于 makefile 的目录

.PHONT: app
app: 
	g++ src/test.cpp $(FLAGS) -o bin/test_skiplist
	./bin/test_skiplist

.PHONY: clean
clean:
	rm -rf bin/*
