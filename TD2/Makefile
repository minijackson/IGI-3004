CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14

all: testIB1.bin testIB2.bin testIB3.bin testIIB.bin testIII1.bin testIII2.bin testIII3.bin

clean:
	$(RM) *.bin *.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

testIB%.bin: testIB%.cpp procutils.o
	$(CXX) $(CXXFLAGS) $^ -o $@

testIIB.bin: testIIB.cpp testIB1.bin procutils.o
	$(CXX) $(CXXFLAGS) $< procutils.o -o $@

testIII%.bin: testIII%.cpp procutils.o gestionFichiersw.o
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: all clean
.SECONDARY: procutils.o gestionFichiersw.o
