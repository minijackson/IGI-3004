#include "pipe.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <system_error>
#include <ios>
#include <cerrno>
#include <cstring>

DummyPipe::DummyPipe(bool nonBlocking) : pipeFd{} {
	if(nonBlocking) {
		if(pipe2(pipeFd, O_NONBLOCK) == -1) {
			throw std::system_error(errno, std::system_category());
		}
	} else {
		if(pipe(pipeFd) == -1) {
			throw std::system_error(errno, std::system_category());
		}
	}
}

Pipe::Pipe(bool nonBlocking)
      : DummyPipe(nonBlocking)
      , IFile(pipeFd[0], 4096)
      , OFile(pipeFd[1], 4096)
      , nonBlocking(nonBlocking) {}

Pipe::~Pipe() {
	close();
}

void Pipe::readOnly() {
	::close(pipeFd[1]);
	writeClosed = true;
}

void Pipe::writeOnly() {
	::close(pipeFd[0]);
	readClosed = true;
}

void Pipe::close() {
	if(!readClosed) {
		::close(pipeFd[0]);
	}

	if(!writeClosed) {
		::close(pipeFd[1]);
	}
}
