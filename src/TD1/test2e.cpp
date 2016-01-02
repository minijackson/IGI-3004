#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 255;

int main() {
	char line[TAILLEBUF];

	try {
		IFile file(0, TAILLEBUF);

		while(!file.hasEnded()) {
			file >> line;
			std::cout << line;
		}

		// file.~IFile();
	} catch(std::ios_base::failure const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
