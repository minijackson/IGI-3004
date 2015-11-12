#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestionFichiersw.hpp"

const size_t TAILLEBUF = 2048;

int main(int argc, char const* argv[]) {
	if(argc != 2) {
		std::cout << "Usage: " << argv[0] << " file" << std::endl;
	} else {
		int fd = open(argv[1], O_RDONLY);
		size_t lineNumber(0);
		if(fd != -1) {
			char line[TAILLEBUF];

			int status;
			do {
				status = firstline(fd, line, TAILLEBUF);
				++lineNumber;
			} while(status);

			close(fd);
			std::cout << "Number of lines: " << lineNumber - 1 << std::endl;
		} else {
			std::cerr << "Error opening file." << std::endl;
			return EBADF;
		}
	}
}
