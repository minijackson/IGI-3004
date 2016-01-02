#include <iostream>

#include "procutils.hpp"

int main() {
	std::cout << "PID du processus: " << getThisProcessID() << std::endl
	          << "PID du processus parent: " << getParentProcessID() << std::endl
	          << "Bonjour" << std::endl;
}
