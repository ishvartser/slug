# $Id: Makefile,v 1.0 2013-05-29 - - $
#
#  Program 4: Google Directions
#  Professor D. Hembold
#  CMPS 101: Algorithms & Abstract Data Types
#  UC Santa Cruz  
#
#  Created by Jason Heron and Igor Shvartser on 5/25/13.
#

all: main graphDriver

main: graph.o list.o
	gcc ${CCOPTS} -std=c99 main.c graph.o list.o -o main 

graphDriver: graph.o list.o
	gcc ${CCOPTS} -std=c99 graphDriver.c graph.o list.o -o graphDriver   

main.o: graph.h list.h main.c
	gcc ${CCOPTS} -std=c99 -c  -Wall main.c

graph.o: graph.h list.h graph.c
	gcc ${CCOPTS} -std=c99 -c  -Wall graph.c

list.o: list.h list.c
	gcc ${CCOPTS} -std=c99 -c  -Wall list.c

graphDriver.o: graphDriver.c graph.h list.h
	gcc ${CCOPTS} -std=c99 -c  -Wall graphDriver.c

clean:
	rm -f store graphDriver main.o graphDriver.o list.o
