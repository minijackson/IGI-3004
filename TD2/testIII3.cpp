#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

#include "procutils.hpp"
#include "gestionFichiersw.hpp"

constexpr const size_t TAILLEBUF = 255;

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
			std::cerr << "fork() a échoué." << std::endl;
			exit(ECHILD);
		} else {
			char line[TAILLEBUF];

			file >> line;
			std::cout << line;

			// file.~IFile();
		}
	} catch(std::ios_base::failure e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return errno;
	}
}
