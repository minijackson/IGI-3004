#include <iostream>

#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/wait.h>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

int main(int argc, char* argv[]) {

	char const* filename;

	if(argc == 1) {
		filename = "fichierTest.txt";
	} else if(argc == 2) {
		filename = argv[1];
	} else {
		std::cerr << "Usage: " << argv[0] << " [filename]" << std::endl;
		exit(E2BIG);
	}

	try {
		IFile file(filename, TAILLEBUF);

		pid_t pid = fork();

		if(pid == -1) {
			std::perror("fork() a échoué");
			exit(errno);
		} else {
			char line[TAILLEBUF];

			file >> line;
			std::cout << line;

			wait(nullptr);

			// file.~IFile();
		}
	} catch(std::ios_base::failure const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
