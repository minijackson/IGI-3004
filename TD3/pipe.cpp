#include "pipe.hpp"

#include <unistd.h>

#include <system_error>
#include <ios>
#include <cerrno>
#include <cstring>

Pipe::Pipe()
  : pipeFd{} {
	if(pipe(pipeFd) == -1) {
		throw std::system_error(errno, std::system_category());
	}
}

Pipe::~Pipe() {
	close(pipeFd[0]);
	close(pipeFd[1]);
}

Pipe& Pipe::operator<<(char const message[]) {
	if(write(pipeFd[1], message, std::strlen(message)) == -1) {
		throw std::ios_base::failure(std::strerror(errno),
		                             std::error_code(errno, std::system_category()));
	}
	return *this;
}

Pipe& Pipe::operator>>(char message[]) {
	char buffer(0);
	int i = 0;
	while((buffer != '\n') && (buffer != 0x04)) {
		if(read(pipeFd[0], &buffer, 1) > 0) {
			message[i] = buffer;
			++i;
		}
	}
	message[i] = '\0';
	return *this;
}
