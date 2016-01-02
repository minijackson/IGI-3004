CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14 -D_GLIBCXX_USE_CXX11_ABI -I../../include/
LDFLAGS = -L../../lib -lgestion-fichiers -lprocutils

all: testIB1.bin testIB2.bin testIB3.bin testIIB.bin testIII1.bin testIII2.bin testIII3.bin

clean:
	$(RM) *.bin *.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

testIB%.bin: testIB%.cpp
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $^ -o $@

testIIB.bin: testIIB.cpp testIB1.bin
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $< -o $@

testIII%.bin: testIII%.cpp
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $^ -o $@

.PHONY: all clean
