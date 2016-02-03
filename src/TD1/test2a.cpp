#include <iostream>
#include <memory>

#include <cerrno>
#include <cstddef>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

int main() {
	try {
		IFile<> file("fichierTest.txt", TAILLEBUF);

		auto line = std::make_unique<char[]>(TAILLEBUF);
		file >> line.get();
		std::cout << line.get();

		// file.~Ifile();
	} catch(std::ios_base::failure const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
