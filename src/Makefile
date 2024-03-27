CXX=g++

CXXFLAGS=-Wall -std=c++17

TARGET=cpm

OBJS=Reader.o CopyModel.o Stats.o cpm.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

Reader.o: Reader.cpp Reader.h
	$(CXX) $(CXXFLAGS) -c Reader.cpp

CopyModel.o: CopyModel.cpp CopyModel.h
	$(CXX) $(CXXFLAGS) -c CopyModel.cpp

Stats.o: Stats.cpp Stats.h
	$(CXX) $(CXXFLAGS) -c Stats.cpp

cpm.o: cpm.cpp Reader.h
	$(CXX) $(CXXFLAGS) -c cpm.cpp

clean:
	rm -f $(OBJS) $(TARGET)
