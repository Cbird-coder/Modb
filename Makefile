CXX=g++

MONGOFACEINC = -I./include

MONGOINC=$(shell pkg-config --cflags libmongocxx)
MONGOLIB=$(shell pkg-config --libs libmongocxx)

CVINC=$(shell pkg-config --cflags opencv)
CVLIB=$(shell pkg-config --libs opencv)

SRC=$(wildcard src/*.cpp)
all:
	@$(CXX) -std=c++11 $(CVINC) $(MONGOFACEINC) $(MONGOINC) $(CVLIB) $(MONGOLIB)  $(SRC) -o mongodb_test
clean:
	@rm -rf mongodb_test
