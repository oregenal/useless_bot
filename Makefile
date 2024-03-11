CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -pedantic -ggdb
BIN=bot
SRCFILES=main.cpp

$(BIN):$(SRCFILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:clean

clean:
	rm -rf $(BIN)
