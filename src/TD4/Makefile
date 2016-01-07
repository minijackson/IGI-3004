CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14 -D_GLIBCXX_USE_CXX11_ABI -I../../include/ -pthread
LDFLAGS = -L../../lib -lgestion-fichiers -pthread

all: testIa.bin testIIb.bin

clean:
	$(RM) *.bin *.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test%.bin: test%.cpp
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $^ -o $@

.PHONY: all clean
