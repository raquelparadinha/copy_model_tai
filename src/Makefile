# Define the compiler
CXX=g++

# Define any compile-time flags
CXXFLAGS=-Wall -std=c++17

# Define the target executable
TARGET=cpm

# Define object files
OBJS=Reader.o cpm.o

# First rule is the one executed when no parameters are fed to the Makefile
all: $(TARGET)

# Rule for linking the program
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule for compiling Reader.cpp to Reader.o
Reader.o: Reader.cpp Reader.h
	$(CXX) $(CXXFLAGS) -c Reader.cpp

# Rule for compiling cpm.cpp to cpm.o
cpm.o: cpm.cpp Reader.h
	$(CXX) $(CXXFLAGS) -c cpm.cpp

# 'make clean' will remove intermediate and executable files
clean:
	rm -f $(OBJS) $(TARGET)
