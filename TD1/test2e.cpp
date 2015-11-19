#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestionFichiersw.hpp"

constexpr const size_t TAILLEBUF = 255;

int main() {
	char line[TAILLEBUF];

	try {
		IFile file(0, TAILLEBUF);

		while(!file.hasEnded()) {
			file >> line;
			std::cout << line;
		}

		// file.~IFile();
	} catch(std::ios_base::failure e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
