#	Make file
#
#	Usage: make -f proj08.makefile
#
#	project 08

proj08: proj08.student.o
	g++ proj08.student.o -o proj08
	
proj08.student.o: proj08.student.c
	g++ -c proj08.student.c

clean:
	rm -f proj08.o
	rm -f proj08
