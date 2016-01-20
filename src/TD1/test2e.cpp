#include <iostream>
#include <memory>

#include <cerrno>
#include <cstddef>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

int main() {
	auto line = std::make_unique<char[]>(TAILLEBUF);

	try {
		IFile file(0, TAILLEBUF);

		while(!file.hasEnded()) {
			file >> line.get();
			std::cout << line.get();
		}

		// file.~IFile();
	} catch(std::ios_base::failure const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
