#include <iostream>
#include <iomanip>

#include <unistd.h>

#include "procutils.hpp"

int main() {
	pid_t pid = fork();

	if(pid == -1) {
		std::cerr << "fork() a échoué." << std::endl;
		exit(ECHILD);
	} else if(pid == 0) {
		std::cout << "Enfant:" << std::endl;
	} else {
		std::cout << "Parent:" << std::endl;
	}

	std::cout << "	- PID du processus: " << std::setw(5) << getThisProcessID()
	          << std::endl
	          << "	- PID du processus parent: " << std::setw(5)
	          << getParentProcessID() << std::endl
	          << "	- bonjour" << std::endl;

	return 0;
}
