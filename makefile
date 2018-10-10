TARGET = cell_distance.c
CC = gcc
LIBS = /home/hpcuser041/lib
INCLUDE = /home/hpcuser041/include
FLAGS = -fopenmp -march=native -std=c11 -Wall -O2 -lm

cell_distance : $(TARGET)
	$(CC) $(TARGET) -I$(INCLUDE) -L$(LIBS) -o $@ $(FLAGS)
