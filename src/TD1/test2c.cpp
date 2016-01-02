#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 255;

int main(int argc, char const* argv[]) {
	if(argc != 2) {
		std::cout << "Usage: " << argv[0] << " file" << std::endl;
	} else {

		try {
			IFile file(argv[1], TAILLEBUF);

			char line[TAILLEBUF];
			while(!file.hasEnded()) {
				file >> line;
				std::cout << line;
			}

			// file.~Ifile();
		} catch(std::ios_base::failure const& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return errno;
		}

	}
}