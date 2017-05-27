# Makefile for listIteratorInt 

CC = gcc
CFLAGS = -Wall -Werror -g
PR = pagerank.o queue.o graph.o urlReader.o
INV = inverted.o set.o urlReader.o

inverted : $(INV)
	$(CC) -o inverted $(INV)

pagerank : $(PR)
	$(CC) -o pagerank $(PR)



pagerank.o: pagerank.c queue.h graph.h urlReader.h
inverted.o: inverted.c urlReader.h set.h

queue.o: queue.c queue.h
graph.o: graph.c graph.h
set.o: set.c set.h
urlReader.o: urlReader.c urlReader.h

clean :
	rm -f inverted pagerank $(INV) $(PR) core *.dSYM







