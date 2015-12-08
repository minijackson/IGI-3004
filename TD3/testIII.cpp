#include <iostream>

#include <cstring>
#include <csignal>

#include <unistd.h>

#include "gestionFichiersw.hpp"
#include "pipe.hpp"

constexpr size_t const TAILLEBUF = 255;

int main() {
	signal(SIGUSR1, [](int sig) {
		if(sig == SIGUSR1) {
			std::cout << "\nToo late: safe closed" << std::endl;
			wait();
			exit(EPERM);
		}
	});

	try {
		pid_t pid = fork();

		if(pid == -1) {
			std::perror("fork() a échoué");
			exit(errno);
		} else if(pid == 0) {
			sleep(10);
			kill(getppid(), SIGUSR1);
			return EPERM;
		} else {
			try {
				char code[TAILLEBUF];
				std::cout << "You have 10 seconds to open the safe." << std::endl
				          << "Please enter the code of the safe: " << std::flush;

				IFile consoleIn(0, TAILLEBUF);
				consoleIn >> code;

				bool invalidCode = std::strcmp(code, "1234\n");

				if(invalidCode) {
					std::cout << "Code incorrect: safe closed" << std::endl;
				} else {
					std::cout << "Code correct: safe opened." << std::endl;
				}

				signal(SIGUSR1, SIG_IGN);
				// Stop the child
				kill(pid, SIGTERM);

				return invalidCode ? EPERM : 0;

			} catch(std::ios_base::failure const& e) {
				std::cerr << "Error: " << e.what() << std::endl;
				return e.code().value();
			}
		}

	} catch(std::system_error const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return e.code().value();
	}
}
