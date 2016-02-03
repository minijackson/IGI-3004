#include <iostream>
#include <memory>
#include <utility>

#include <cerrno>
#include <cstddef>
#include <cstdlib>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

int main(int argc, char const* argv[]) {
	if(argc != 2) {
		std::cout << "Usage: " << argv[0] << " file" << std::endl;
		exit(EINVAL);
	} else {
		try {
			IFile<> file(std::move(argv[1]), TAILLEBUF);

			auto line = std::make_unique<char[]>(TAILLEBUF);
			file >> line.get();
			std::cout << line.get();

			// file.~Ifile();
		} catch(std::ios_base::failure const& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return errno;
		}
	}
}

