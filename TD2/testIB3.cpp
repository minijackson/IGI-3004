#include <iostream>
#include <iomanip>

#include <unistd.h>
#include <sys/wait.h>

#include "procutils.hpp"

int main() {
	pid_t pid = fork();

	if(pid == -1) {
		std::perror("fork() a échoué");
		exit(errno);
	} else if(pid == 0) {
		std::cout << "Enfant:" << std::endl;
	} else {
		int status(0);
		wait(&status);

		if(!WIFEXITED(status)) {
			std::cerr << "Erreur: le processus enfant n'a pas quitté correctement" << std::endl;
			exit(1);
		}

		std::cout << "Parent:" << std::endl;
	}

	std::cout << "	- PID du processus: " << std::setw(5) << getThisProcessID() << std::endl
	          << "	- PID du processus parent: " << std::setw(5) << getParentProcessID()
	          << std::endl
	          << "	- bonjour" << std::endl;

	return 0;
}
