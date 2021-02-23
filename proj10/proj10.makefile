#	Make file
#
#	Usage: make -f proj10.makefile
#
#	project 10

proj10: proj10.student.o
	g++ proj10.student.o -o proj10
	
proj10.student.o: proj10.student.c
	g++ -c proj10.student.c

clean:
	rm -f proj10.o
	rm -f proj10
