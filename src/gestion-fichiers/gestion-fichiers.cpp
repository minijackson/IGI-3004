#include "gestion-fichiers.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <ios>

IFile::IFile(int const fd, size_t const bufSize)
  : fd(fd)
  , bufSize(bufSize) {}

IFile::IFile(char const* filename, size_t const bufSize)
  : fd(openFile(filename))
  , bufSize(bufSize) {}

IFile::~IFile() {
	if(openedTheFile) {
		close(fd);
	}
}

IFile& IFile::operator>>(char* line) {
	getLine(line);
	return *this;
}

bool IFile::hasEnded() {
	return ended;
}

int IFile::openFile(char const* filename) {
	int fd = open(filename, O_RDONLY);
	if(fd == -1) {
		throw std::ios_base::failure("Cannot open file for reading",
		                             std::error_code(errno, std::system_category()));
	}
	openedTheFile = true;
	return fd;
}

void IFile::getLine(char* buf) {
	size_t i = 0;
	do {
		// Crash! Line too long
		if(i >= bufSize) {
			buf[0] = '\0';
			ended = true;
			return;
		}

		int status = read(fd, &buf[i], 1);
		if(status == 0) {
			ended = true;
			break;
		} else if(status > 0) {
			++i;
		} else {
			// If nothing is in the pipe and the pipe is non-blocking
			throw std::system_error(errno, std::system_category());
		}
	} while(buf[i-1] != '\n');
	buf[i] = '\0';
}

OFile::OFile(int const fd, size_t const bufSize)
  : fd(fd)
  , bufSize(bufSize) {}

OFile::OFile(char const* filename, size_t const bufSize)
  : fd(openFile(filename))
  , bufSize(bufSize) {}

OFile::~OFile() {
	if(openedTheFile) {
		close(fd);
	}
}

OFile& OFile::operator<<(char const* line) {
	writeLine(line);
	return *this;
}

int OFile::openFile(char const* filename) {
	int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if(fd == -1) {
		throw std::ios_base::failure("Cannot open file for writing",
		                             std::error_code(errno, std::system_category()));
	}
	return fd;
}

void OFile::writeLine(char const* content) {
	size_t len = std::strlen(content);
	if(len > bufSize) {
		len = bufSize;
	}

	if(write(fd, content, len) == -1) {
		throw std::ios_base::failure("Cannot write in file",
		                             std::error_code(errno, std::system_category()));
	}
}
