# Makefile for listIteratorInt 

CC = gcc
CFLAGS = -Wall -Werror -g
PR = pagerank.o queue.o graph.o urlReader.o set.o
INV = inverted.o set.o urlReader.o
SPR = searchPagerank.o set.o urlReader.o
SFR = scaledFootrule.o set.o urlReader.o
STFIDF = searchTfIdf.o set.o urlReader.o
EXEC = inverted pagerank searchPagerank searchTfIdf scaledFootrule


all: $(EXEC)

inverted : $(INV)
	$(CC) -o inverted $(INV)

pagerank : $(PR)
	$(CC) -o pagerank $(PR)

searchPagerank: $(SPR)
	$(CC) -o searchPagerank $(SPR)

searchTfIdf: $(STFIDF)
	$(CC) -o searchTfIdf $(STFIDF)

scaledFootrule: $(SFR)
	$(CC) -o scaledFootrule $(SFR)

pagerank.o: pagerank.c queue.h graph.h urlReader.h
inverted.o: inverted.c urlReader.h set.h
searchPagerank.o: searchPagerank.c set.h urlReader.h
searchTfIdf.o: searchTfIdf.c set.h urlReader.h
scaledFootrule: scaledFootrule.c set.h urlReader.h

queue.o: queue.c queue.h
graph.o: graph.c graph.h
set.o: set.c set.h
urlReader.o: urlReader.c urlReader.h

clean :
	rm -f $(EXEC) $(SFR) $(STFIDF) $(SPR) $(INV) $(PR) core *.dSYM