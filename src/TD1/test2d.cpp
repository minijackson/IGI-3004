#include <iostream>
#include <memory>
#include <utility>

#include <cerrno>
#include <cstddef>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

int main(int argc, char const* argv[]) {
	if(argc != 2) {
		std::cout << "Usage: " << std::move(argv[0]) << " file" << std::endl;
	} else {
		try {
			IFile<> file(std::move(argv[1]), TAILLEBUF);

			auto line       = std::make_unique<char[]>(TAILLEBUF);
			auto lineNumber = int{0};

			while(!file.hasEnded()) {
				file >> line.get();
				++lineNumber;
			}

			std::cout << "Number of lines: " << lineNumber - 1 << std::endl;

			// file.~Ifile();
		} catch(std::ios_base::failure const& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return errno;
		}
	}
}
