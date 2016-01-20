#include <iostream>
#include <memory>

#include <cerrno>
#include <cstddef>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

int main() {
	try {
		IFile readingFile(1, TAILLEBUF);
		OFile writingFile(0, TAILLEBUF);

		auto line = std::make_unique<char[]>(TAILLEBUF);

		while(!readingFile.hasEnded()) {
			readingFile >> line.get();
			writingFile << line.get();
		}
	} catch(std::ios_base::failure const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
