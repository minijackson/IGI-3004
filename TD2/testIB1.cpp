#include <iostream>

#include "procutils.hpp"

int main() {
	std::cout << getThisProcessID() << ", " << getParentProcessID()
	          << " bonjour" << std::endl;
}
