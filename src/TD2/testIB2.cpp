#include <iostream>
#include <iomanip>

#include <cerrno>
#include <cstdio>
#include <cstdlib>

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
		std::cout << "Parent:" << std::endl;
	}

	std::cout << "	- PID du processus: " << std::setw(5) << getThisProcessID() << std::endl
	          << "	- PID du processus parent: " << std::setw(5) << getParentProcessID()
	          << std::endl
	          << "	- bonjour" << std::endl;

	wait(nullptr);

	return 0;
}
