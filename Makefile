CXX=g++
CXXFLAGS=-Wall -std=c++17

BIN=bin
SRC=src

CPM=cpm
TUNING=cpm_tunning
MUTATE=mutate

# Define object files
OBJS_CPM=$(BIN)/Reader.o $(BIN)/CopyModel.o $(BIN)/Stats.o $(BIN)/cpm.o
OBJS_TUNING=$(BIN)/Reader.o $(BIN)/CopyModel.o $(BIN)/Stats.o $(BIN)/cpm_tunning.o
OBJS_MUTATE=$(BIN)/mutate.o


# First rule is the one executed when no parameters are fed to the Makefile
all: $(CPM) $(TUNING) $(MUTATE)

$(CPM): $(OBJS_CPM)
	$(CXX) $(CXXFLAGS) -o $(BIN)/$(CPM) $(OBJS_CPM)

$(TUNING): $(OBJS_TUNING)
	$(CXX) $(CXXFLAGS) -o $(BIN)/$(TUNING) $(OBJS_TUNING)

$(MUTATE): $(OBJS_MUTATE)
	$(CXX) $(CXXFLAGS) -o $(BIN)/$(MUTATE) $(OBJS_MUTATE)

$(BIN)/Reader.o: $(SRC)/Reader.cpp $(SRC)/Reader.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/Reader.cpp -o $(BIN)/Reader.o

$(BIN)/CopyModel.o: $(SRC)/CopyModel.cpp $(SRC)/CopyModel.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/CopyModel.cpp -o $(BIN)/CopyModel.o

$(BIN)/Stats.o: $(SRC)/Stats.cpp $(SRC)/Stats.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/Stats.cpp -o $(BIN)/Stats.o

$(BIN)/cpm.o: $(SRC)/cpm.cpp $(SRC)/Reader.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/cpm.cpp -o $(BIN)/cpm.o

$(BIN)/cpm_tunning.o: $(SRC)/cpm_tunning.cpp $(SRC)/Reader.h
	$(CXX) $(CXXFLAGS) -c $(SRC)/cpm_tunning.cpp -o $(BIN)/cpm_tunning.o

$(BIN)/mutate.o: $(SRC)/mutate.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/mutate.cpp -o $(BIN)/mutate.o

# 'make clean' will remove intermediate and executable files
clean:
	rm -f $(OBJS_CPM) $(OBJS_TUNING) $(OBJS_MUTATE) $(BIN)/$(CPM) $(BIN)/$(TUNING) $(BIN)/$(MUTATE)
