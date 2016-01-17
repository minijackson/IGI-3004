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
      , bufSize(bufSize)
      , openedTheFile(true) {}

IFile::~IFile() {
	if(openedTheFile) {
		close(fd);
	}
}

IFile& IFile::operator>>(char* line) {
	getLine(line);
	return *this;
}

int IFile::getFd() const {
	return fd;
}

size_t IFile::getBufSize() const {
	return bufSize;
}

bool IFile::hasEnded() const {
	return ended;
}

bool IFile::hasOpenedThefile() const {
	return openedTheFile;
}

int IFile::openFile(char const* filename) {
	int fd = open(filename, O_RDONLY);
	if(fd == -1) {
		throw std::ios_base::failure("Cannot open file for reading",
		                             std::error_code(errno, std::system_category()));
	}
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

		ssize_t status = read(fd, &buf[i], 1);
		if(status == 0) {
			ended = true;
			break;
		} else if(status > 0) {
			++i;
		} else {
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
      , bufSize(bufSize)
      , openedTheFile(true) {}

OFile::~OFile() {
	if(openedTheFile) {
		close(fd);
	}
}

OFile& OFile::operator<<(char const* line) {
	writeLine(line);
	return *this;
}

int OFile::getFd() const {
	return fd;
}

size_t OFile::getBufSize() const {
	return bufSize;
}

bool OFile::hasOpenedThefile() const {
	return openedTheFile;
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
