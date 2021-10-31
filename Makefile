# CC=gcc
# CFLAGS=-I.
CXX=clang++

prog2: prog2.o
	${CXX} prog2.o -o prog2

prog2.o: prog2.cpp
	${CXX} -c prog2.cpp -o prog2.o
