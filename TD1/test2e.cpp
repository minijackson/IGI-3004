#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestionFichiersw.hpp"

const size_t TAILLEBUF = 255;

int main() {
	int fd = open(0, O_RDONLY);
	if(fd != -1) {
		char line[TAILLEBUF];

		int status;
		do {
			status = firstline(fd, line, TAILLEBUF);
			std::cout << line;
		} while(status);

		close(fd);
	} else {
		std::cerr << "Error opening file." << std::endl;
		return EBADF;
	}
}
