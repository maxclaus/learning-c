server:
	gcc -g -o server Server.c main.c
	./server

# Use valgrind to check for memory leaks in our program.
valgrind:
	gcc -g -o server Server.c main.c
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./server
