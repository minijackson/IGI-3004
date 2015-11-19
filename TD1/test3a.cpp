#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <iostream>

#include "gestionFichiersw.hpp"

constexpr const size_t TAILLEBUF = 255;

int main(int argc, char const* argv[]) {
	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " source destination" << std::endl;
	} else {
		try {
			IFile readingFile(argv[1], TAILLEBUF);
			OFile writingFile(argv[2], TAILLEBUF);
			char line[TAILLEBUF];

			while(!readingFile.hasEnded()) {
				readingFile >> line;
				writingFile << line;
			}

			// readingFile.~Ifile();
			// writingFile.~Ofile();
		} catch(std::ios_base::failure e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return errno;
		}
	}
}
