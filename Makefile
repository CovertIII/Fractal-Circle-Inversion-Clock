CFLAGS = -g
MACFLAGS = -framework GLUT -framework OpenGL -framework Cocoa

CC = gcc

all: circle

circle: circle.o vector2.o
	$(CC) $(CFLAGS) circle.o $(MACFLAGS) vector2.o -o circle 

circle.o: circle.c vector2.h
	$(CC) $(CFLAGS) -c circle.c

vector2.o: vector2.c vector2.h
	$(CC) $(CFLAGS) -c vector2.c

clean:
	\rm *.o
	
