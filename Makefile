# Makefile for listIteratorInt 

CC = gcc
CFLAGS = -Wall -Werror -g
OBJS = pagerank.o queue.o graph.o urlReader.o

pagerank : $(OBJS)
	$(CC) -o pagerank $(OBJS)

pagerank.o: pagerank.c queue.h graph.h urlReader.h
queue.o: queue.c queue.h
graph.o: graph.c graph.h
urlReader.o: urlReader.c urlReader.h

clean :
	rm -f pagerank $(OBJS) core *.dSYM







