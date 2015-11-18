#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

#include "procutils.hpp"

int main() {
	pid_t pid = fork();

	if(pid == -1) {
		std::cerr << "fork() a échoué." << std::endl;
		exit(ECHILD);
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

	std::cout << "	- " << getThisProcessID() << ", " << getParentProcessID()
	          << " bonjour" << std::endl;

	return 0;
}
