CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14

all: testI2.bin

clean:
	$(RM) *.bin *.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test%.bin: test%.cpp pipe.o gestionFichiersw.o
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: all clean
.SECONDARY: pipe.o gestionFichiersw.o
