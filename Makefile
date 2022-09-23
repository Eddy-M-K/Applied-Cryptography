# Makefile for the Applied Cryptography Project
CXX=g++
CXXFLAGS = -Wall -std=c++17
RM=rm -f
TYPES_LIB=types_bin.o types_hex.o types_b64.o
SRCS=cryptopals_tests.cpp types_bin.cpp types_hex.cpp types_b64.cpp
OBJS=cryptopals_tests.o $(TYPES_LIB)
TARGETS=main.out

all: $(TARGETS)

main.out: $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS)

.PHONY: clean
clean:
	$(RM) $(TARGETS) $(OBJS)
