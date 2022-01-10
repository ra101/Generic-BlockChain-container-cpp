CXX=g++
CXXFLAGS=-std=c++11

build: main.cpp block_chain.hpp block.hpp transaction.hpp includes/rsa.hpp includes/prime.hpp
	@echo "\n\nBuilding..."
	$(CXX) $(CXXFLAGS) -o block_chain.out main.cpp

run: block_chain.out
	@echo "\n\nExecuting..."
	./block_chain.out

clean: block_chain.out
	@echo "\n\nCleaning..."
	rm block_chain.out

all: build run clean
