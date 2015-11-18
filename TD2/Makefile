CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14

all: testIB1.bin testIB2.bin testIB3.bin

clean:
	$(RM) *.bin *.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test%.bin: test%.cpp procutils.o
	$(CXX) $(CXXFLAGS) $< procutils.o -o $@

.PHONY: all clean
.SECONDARY: procutils.o
