#	Make file
#
#	Usage: make -f proj12.server.makefile
#
#	project 12

proj12.server: proj12.server.o
	g++ proj12.server.o -o proj12.server
	
proj12.server.o: proj12.server.c
	g++ -c proj12.server.c

clean:
	rm -f proj12.server.o
	rm -f proj12.server
