#include <iostream>

#include <cerrno>
#include <cstddef>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

int main() {
	try {
		OFile file(1, TAILLEBUF);
		file << "Hello, World!\n";
	} catch(std::ios_base::failure const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
