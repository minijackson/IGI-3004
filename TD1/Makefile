CXXFLAGS= -Wall -Wextra -march=native -pipe

all: test2a.bin test2b.bin test2c.bin

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test%.bin: test%.cpp gestionFichiersw.o
	$(CXX) $(CXXFLAGS) $< gestionFichiersw.o -o $@

.PHONY: all
