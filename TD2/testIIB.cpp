#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

#include "procutils.hpp"

int main() {
	pid_t pid = fork();

	if(pid == -1) {
		std::perror("fork() a échoué");
		exit(errno);
	} else if(pid == 0) {
		char commandName[] = "./testIB1.bin";
		char* const command[] = {commandName, nullptr};
		execve(commandName, command, environ);
	} else {
		int status(0);
		wait(&status);

		if(!WIFEXITED(status)) {
			std::cerr << "Erreur: le processus enfant n'a pas quitté correctement" << std::endl;
			exit(1);
		}

		std::cout << getThisProcessID() << " Je suis le père" << std::endl;
	}
	return 0;
}
