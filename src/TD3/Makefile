CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14 -D_GLIBCXX_USE_CXX11_ABI -I../../include/
LDFLAGS = -L../../lib -lgestion-fichiers -lpipe

all: testI2.bin testIII.bin

debug: CXXFLAGS += -ggdb3
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	$(RM) *.bin *.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test%.bin: test%.cpp
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $^ -o $@

.PHONY: all debug release clean
