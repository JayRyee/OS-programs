#	Make file
#
#	Usage: 
#
#	project 07

proj07: proj07.student.o
	g++ proj07.student.o -o proj07
	
proj07.student.o: proj07.student.c
	g++ -c proj07.student.c

clean:
	rm -f proj07.o
	rm -f proj07
