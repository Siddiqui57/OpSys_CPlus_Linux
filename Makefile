CXX = g++
CXXFLAGS = -Wall -Werror -ansi -pedantic -std=c++14 -g

# Default target to build the executable
all: ncc

# Rule to link object files into the final executable
ncc: main.o lex.o buffio.o token.o error.o tree.o machine.o
	$(CXX) $(CXXFLAGS) -o ncc main.o lex.o buffio.o token.o error.o tree.o machine.o

# Compilation rules for each source file
main.o: main.cpp lex.h buffio.h token.h error.h tree.h machine.h
	$(CXX) $(CXXFLAGS) -c main.cpp

lex.o: lex.cpp lex.h buffio.h token.h error.h
	$(CXX) $(CXXFLAGS) -c lex.cpp

buffio.o: buffio.cpp buffio.h
	$(CXX) $(CXXFLAGS) -c buffio.cpp

error.o: error.cpp error.h
	$(CXX) $(CXXFLAGS) -c error.cpp

token.o: token.cpp token.h
	$(CXX) $(CXXFLAGS) -c token.cpp

tree.o: tree.cpp tree.h lex.h buffio.h token.h error.h
	$(CXX) $(CXXFLAGS) -c tree.cpp

machine.o: machine.cpp machine.h tree.h
	$(CXX) $(CXXFLAGS) -c machine.cpp

# Clean rule to remove generated files
clean:
	rm -f ncc main.o lex.o buffio.o token.o error.o tree.o machine.o

