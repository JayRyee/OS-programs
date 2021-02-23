#	Make file
#
#	Usage: 
#
#	project 06

proj06: proj06.student.o
	g++ proj06.student.o -o proj06 -lpthread
	
proj06.student.o: proj06.student.c
	g++ -c proj06.student.c

clean:
	rm -f proj06.o
	rm -f proj06
