OBJS	= automata.o main.o parser.o
SOURCE	= automata.cpp main.cpp parser.cpp
HEADER	= automata.hpp parser.h
OUT	= minimizar_automato
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

automata.o: automata.cpp automata.hpp
	$(CC) $(FLAGS) automata.cpp

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

parser.o: parser.cpp parser.h
	$(CC) $(FLAGS) parser.cpp


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
