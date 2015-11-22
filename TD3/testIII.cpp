#include <iostream>

#include <cstring>
#include <csignal>

#include <unistd.h>

#include "gestionFichiersw.hpp"
#include "pipe.hpp"

constexpr size_t const TAILLEBUF = 255;

int main() {
	try {
		Pipe myPipe(/* nonBlocking = */ true);
		pid_t pid = fork();

		if(pid == -1) {
			std::perror("fork() a échoué");
			exit(errno);
		} else if(pid == 0) {
			char status[TAILLEBUF];

			sleep(10);

			myPipe >> status;

			if(std::strcmp(status, "Finished\n")) {
				std::cout << "Too late: safe closed." << std::endl;
				myPipe << "Too late\n";
				return EPERM;
			}

		} else {
			try {
				char code[TAILLEBUF];
				std::cout << "You have 10 seconds to open the safe." << std::endl
				          << "Please enter the code of the safe: " << std::flush;

				IFile consoleIn(0, TAILLEBUF);
				consoleIn >> code;

				char status[TAILLEBUF];
				myPipe >> status;
				myPipe << "Finished\n";

				bool invalidCode = std::strcmp(code, "1234\n"),
				     notTooLate = std::strcmp(status, "Too late\n");
				if(notTooLate) {
					if(invalidCode) {
						std::cout << "Code incorrect: safe closed" << std::endl;
					} else {
						std::cout << "Code correct: safe opened." << std::endl;
					}
				}

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
