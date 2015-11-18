#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestionFichiersw.hpp"

const size_t TAILLEBUF = 255;

int main() {
	try {
		OFile file(1, TAILLEBUF);
		file << "Hello, World!\n";
	} catch(std::ios_base::failure e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
