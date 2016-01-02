#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 255;

int main() {
	try {
		IFile readingFile(1, TAILLEBUF);
		OFile writingFile(0, TAILLEBUF);
		char line[TAILLEBUF];

		while(!readingFile.hasEnded()) {
			readingFile >> line;
			writingFile << line;
		}
	} catch(std::ios_base::failure const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
