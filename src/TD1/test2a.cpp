#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 255;

int main() {
	try {
		IFile file("fichierTest.txt", TAILLEBUF);

		char line[TAILLEBUF];
		file >> line;
		std::cout << line;

		// file.~Ifile();
	} catch(std::ios_base::failure const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
