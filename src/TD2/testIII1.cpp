#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

#include "procutils.hpp"
#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 255;

int main() {
	pid_t pid = fork();

	if(pid == -1) {
		std::perror("fork() a échoué");
		exit(errno);
	} else {
		try {
			IFile file("fichierTest.txt", TAILLEBUF);

			char line[TAILLEBUF];

			file >> line;
			std::cout << line;

			// file.~IFile();
		} catch(std::ios_base::failure const& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return errno;
		}
	}
}
