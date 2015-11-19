#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestionFichiersw.hpp"

constexpr const size_t TAILLEBUF = 255;

int main() {
	try {
		IFile file("fichierTest.txt", TAILLEBUF);

		char line[TAILLEBUF];
		file >> line;
		std::cout << line;

		// file.~Ifile();
	} catch(std::ios_base::failure e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
