#include "gestionFichiersw.hpp"

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
	close(fd);
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
		throw std::ios_base::failure(std::strerror(errno));
	}
	return fd;
}

void IFile::getLine(char* buf) {
	off_t currentPos = lseek(fd, 0, SEEK_CUR);
	int status = read(fd, buf, bufSize);

	if (status == 0) {
		buf[0] = '\0';
		ended = true;
	} else if(status != -1) {
		for(size_t i = 0 ; i < bufSize ; ++i) {
			if(buf[i] == '\n') {
				lseek(fd, currentPos + ((i+1)*sizeof(char)), SEEK_SET);
				buf[i+1] = '\0';
				break;
			}
		}
	} else {
		throw std::ios_base::failure(std::strerror(errno));
	}
}

OFile::OFile(int const fd, size_t const bufSize)
  : fd(fd)
  , bufSize(bufSize) {}

OFile::OFile(char const* filename, size_t const bufSize)
  : fd(openFile(filename))
  , bufSize(bufSize) {}

OFile::~OFile() {
	close(fd);
}

OFile& OFile::operator<<(char const* line) {
	writeLine(line);
	return *this;
}

int OFile::openFile(char const* filename) {
	int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if(fd == -1) {
		throw std::ios_base::failure(std::strerror(errno));
	}
	return fd;
}

void OFile::writeLine(char const* content) {
	size_t len = std::strlen(content);
	if(len > bufSize) {
		len = bufSize;
	}

	if(write(fd, content, len) == -1) {
		throw std::ios_base::failure(std::strerror(errno));
	}
}
