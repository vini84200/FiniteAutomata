OBJS	= automato.o main.o parser.o
SOURCE	= automato.cpp main.cpp parser.cpp
HEADER	= automato.hpp parser.h
OUT	= minimizar_automato
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -std=c++11 -static -static-libgcc -static-libstdc++

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

automato.o: automato.cpp automato.hpp
	$(CC) $(FLAGS) automato.cpp

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

parser.o: parser.cpp parser.h
	$(CC) $(FLAGS) parser.cpp


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
