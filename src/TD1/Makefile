CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14 -D_GLIBCXX_USE_CXX11_ABI -I../../include/
LDFLAGS = -L../../lib -lgestion-fichiers

all: test2a.bin test2b.bin test2c.bin test2d.bin test2e.bin test3a.bin test3b.bin test3c.bin

clean:
	$(RM) *.bin *.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test%.bin: test%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@

.PHONY: all clean
