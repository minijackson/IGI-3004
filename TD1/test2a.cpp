#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <iostream>

#include "gestionFichiersw.hpp"

const size_t TAILLEBUF = 255;

int main() {
	int fd = open("fichierTest.txt", O_RDONLY);
	if(fd != -1) {
		char line[TAILLEBUF];
		firstline(fd, line, TAILLEBUF);
		std::cout << line;
		close(fd);
	} else {
		std::cerr << "Error opening file." << std::endl;
		return EBADF;
	}
}
