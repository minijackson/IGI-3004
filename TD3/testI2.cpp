#include <iostream>

#include <cstdio>
#include <cstring>

#include <unistd.h>

#include "pipe.hpp"
#include "gestionFichiersw.hpp"

constexpr size_t const TAILLEBUF            = 255;
constexpr char const SECRET_INDICATOR[]     = "secret: ";
constexpr size_t const SECRET_INDICATOR_LEN = 8;

int main(int argc, char* argv[]) {

	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " source destination" << std::endl;
		return EINVAL;
	}

	try {
		Pipe myPipe;
		pid_t pid = fork();

		if(pid == -1) {
			std::perror("fork() a échoué");
			exit(errno);
		} else if(pid == 0) {
			myPipe.readOnly();
			try {
				OFile destinationFile(argv[2], TAILLEBUF);

				char receivedMessage[TAILLEBUF];
				myPipe >> receivedMessage;

				// Character 0x04 is EOT (end of transmission)
				while(receivedMessage[0] != 0x04) {
					destinationFile << receivedMessage;
					myPipe >> receivedMessage;
				}
			} catch(std::ios_base::failure const& e) {
				std::cerr << "Error: " << e.what() << std::endl;
				return e.code().value();
			}
		} else {
			myPipe.writeOnly();

			// Character 0x04 is EOT (end of transmission)
			char const endMessage[] = {0x04, '\0'};

			try {
				IFile sourceFile(argv[1], TAILLEBUF);
				char messageToTransmit[TAILLEBUF];

				while(!sourceFile.hasEnded()) {
					sourceFile >> messageToTransmit;
					if(std::strncmp(messageToTransmit, SECRET_INDICATOR, SECRET_INDICATOR_LEN)) {
						myPipe << messageToTransmit;
					}
				}
				myPipe << endMessage;

			} catch(std::ios_base::failure const& e) {
				myPipe << endMessage;
				std::cerr << "Error: " << e.what() << std::endl;
				return e.code().value();
			}

		}
	} catch(std::system_error const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return e.code().value();
	}
}
