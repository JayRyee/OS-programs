#	Make file
#
#	Usage: make -f proj11.client.makefile
#
#	project 12

proj12.client: proj12.client.o
	g++ proj12.client.o -o proj12.client
	
proj12.client.o: proj12.client.c
	g++ -c proj12.client.c

clean:
	rm -f proj12.client.o
	rm -f proj12.client
