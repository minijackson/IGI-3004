CXXFLAGS= -Wall -Wextra -march=native -pipe -std=c++14 -D_GLIBCXX_USE_CXX11_ABI -I../gestion-fichiers

all: libpipe.so

debug: CXXFLAGS += -ggdb3
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	$(RM) *.so *.os

%.os: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

lib%.so: %.os
	$(CXX) -shared -o $@ $^

.PHONY: all debug release clean
.SECONDARY: pipe.os
