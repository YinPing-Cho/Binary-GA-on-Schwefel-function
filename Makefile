	# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g
# the build target executable:
TARGET = main

all: $(TARGET)

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o GA.o
	$(CXX) $(CXXFLAGS) -o main main.o GA.o

# The main.o target can be written more simply

main.o: main.cpp Class.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Class.o: Class.h
	$(CXX) $(CXXFLAGS) -c Class.h

GA.o: Class.h GA.cpp
	$(CXX) $(CXXFLAGS) -c GA.cpp

clean: 
	$(RM) count *.o *~