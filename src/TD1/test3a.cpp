#include <iostream>
#include <memory>
#include <utility>

#include <cerrno>
#include <cstddef>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

int main(int argc, char const* argv[]) {
	if(argc != 3) {
		std::cout << "Usage: " << std::move(argv[0]) << " source destination" << std::endl;
	} else {
		try {
			IFile readingFile(std::move(argv[1]), TAILLEBUF);
			OFile writingFile(std::move(argv[2]), TAILLEBUF);

			auto line = std::make_unique<char[]>(TAILLEBUF);

			while(!readingFile.hasEnded()) {
				readingFile >> line.get();
				writingFile << line.get();
			}

			// readingFile.~Ifile();
			// writingFile.~Ofile();
		} catch(std::ios_base::failure const& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return errno;
		}
	}
}
