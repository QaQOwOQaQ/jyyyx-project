.PHONY:sender
sender:
	gcc _sender.c -o sender -Wall -Werror -g 
	./sender 255.255.255.255 9190

.PHONY:recever
recever:
	gcc _recever.c -o recever -Wall -Werror -g 
	./recever 9190

.PHONY:clean
clean:
	rm -rf recever sender *.o a.out
	