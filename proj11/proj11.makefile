#	Make file
#
#	Usage: make -f proj11.makefile
#
#	project 11

proj11: proj11.student.o
	g++ proj11.student.o -o proj11
	
proj11.student.o: proj11.student.c
	g++ -c proj11.student.c

clean:
	rm -f proj11.o
	rm -f proj11
