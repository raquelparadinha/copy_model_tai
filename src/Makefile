CXX=g++

CXXFLAGS=-Wall -std=c++17

TARGET=cpm

TARGET_TUNING=cpm_tunning

# Define object files
OBJS=Reader.o CopyModel.o Stats.o cpm.o

OBJS_TUNING=Reader.o CopyModel.o Stats.o cpm_tunning.o


# First rule is the one executed when no parameters are fed to the Makefile
all: $(TARGET) $(TARGET_TUNING)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(TARGET_TUNING): $(OBJS_TUNING)
	$(CXX) $(CXXFLAGS) -o $(TARGET_TUNING) $(OBJS_TUNING)

# Rule for compiling Reader.cpp to Reader.o
Reader.o: Reader.cpp Reader.h
	$(CXX) $(CXXFLAGS) -c Reader.cpp

CopyModel.o: CopyModel.cpp CopyModel.h
	$(CXX) $(CXXFLAGS) -c CopyModel.cpp

Stats.o: Stats.cpp Stats.h
	$(CXX) $(CXXFLAGS) -c Stats.cpp

cpm.o: cpm.cpp Reader.h
	$(CXX) $(CXXFLAGS) -c cpm.cpp

# New rule for compiling cpm_tunning.cpp to cpm_tunning.o
cpm_tunning.o: cpm_tunning.cpp Reader.h
	$(CXX) $(CXXFLAGS) -c cpm_tunning.cpp

# 'make clean' will remove intermediate and executable files
clean:
	rm -f $(OBJS) $(OBJS_TUNING) $(TARGET) $(TARGET_TUNING)
