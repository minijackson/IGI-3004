CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14

all: test2a.bin test2b.bin test2c.bin test2d.bin test2e.bin

clean:
	$(RM) *.bin *.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test%.bin: test%.cpp gestionFichiersw.o
	$(CXX) $(CXXFLAGS) $< gestionFichiersw.o -o $@

.PHONY: all clean
