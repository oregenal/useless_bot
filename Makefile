CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++14 -pedantic -ggdb
BIN=bot
SRCFILES=main.cpp

$(BIN):$(SRCFILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:clean

clean:
	rm -rf $(BIN)
