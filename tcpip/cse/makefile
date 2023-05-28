.PHONY:server
server:
	gcc _server.c -o server -Wall -Werror -g 
	./server 9190

.PHONY:client
client:
	gcc _client.c -o client -Wall -Werror -g 
	./client 127.0.0.1 9190

.PHONY:clean
clean:
	rm -rf client server *.o a.out