#include "procutils.hpp"

#include <unistd.h>

int getThisProcessID() {
	return getpid();
}

int getParentProcessID() {
	return getppid();
}
