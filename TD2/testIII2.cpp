#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

#include "procutils.hpp"
#include "gestionFichiersw.hpp"

constexpr const size_t TAILLEBUF = 255;

int main() {
	try {
		IFile file("fichierTest.txt", TAILLEBUF);

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
