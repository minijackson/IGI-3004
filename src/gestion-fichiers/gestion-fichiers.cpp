#include "gestion-fichiers.hpp"

#include <ios>
#include <system_error>

#include <cerrno>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

IFile::IFile() noexcept
      : dummy(true)
      , openedTheFile(false) {}

IFile::IFile(int const fd, size_t const bufSize) noexcept
      : fd(fd)
      , bufSize(bufSize) {}

IFile::IFile(char const* filename, size_t const bufSize)
      : fd(openFile(filename))
      , bufSize(bufSize)
      , openedTheFile(true) {}

IFile::~IFile() {
	close();
}

IFile::IFile(IFile&& other) noexcept
      : dummy(other.dummy),
        fd(other.fd),
        bufSize(other.bufSize),
        ended(other.ended),
        openedTheFile(other.openedTheFile) {
	other.dummy         = true;
	other.openedTheFile = false;
}

IFile& IFile::operator=(IFile&& other) noexcept {
	this->dummy = other.dummy;
	this->fd = other.fd;
	this->bufSize = other.bufSize;
	this->ended = other.ended;
	this->openedTheFile = other.openedTheFile;

	other.dummy         = true;
	other.openedTheFile = false;
	return *this;
}

IFile& IFile::operator>>(char* line) {
	if(!dummy) {
		getLine(line);
	}
	return *this;
}

void IFile::close() {
	if(openedTheFile) {
		::close(fd);
		openedTheFile = false;
	}
	dummy = true;
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

bool IFile::hasOpenedTheFile() const {
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
	if(!dummy) {
		size_t i = 0;
		do {
			// Crash! Line too long
			if(i >= bufSize) {
				buf[0] = '\0';
				ended  = true;
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
		} while(buf[i - 1] != '\n');
		buf[i] = '\0';
	}
}

OFile::OFile() noexcept
      : dummy(true)
      , openedTheFile(false) {}

OFile::OFile(int const fd, size_t const bufSize) noexcept
      : fd(fd)
      , bufSize(bufSize) {}

OFile::OFile(char const* filename, size_t const bufSize)
      : fd(openFile(filename))
      , bufSize(bufSize)
      , openedTheFile(true) {}

OFile::~OFile() {
	close();
}

OFile::OFile(OFile&& other) noexcept
      : dummy(other.dummy),
        fd(other.fd),
        bufSize(other.bufSize),
        openedTheFile(other.openedTheFile) {
	other.dummy = true;
}

OFile& OFile::operator=(OFile&& other) noexcept {
	this->dummy = other.dummy;
	this->fd = other.fd;
	this->bufSize = other.bufSize;
	this->openedTheFile = other.openedTheFile;

	other.dummy = true;
	return *this;
}

OFile& OFile::operator<<(char const* line) {
	writeLine(line);
	return *this;
}

void OFile::close() {
	if(openedTheFile) {
		::close(fd);
	}
	dummy = true;
}

int OFile::getFd() const {
	return fd;
}

size_t OFile::getBufSize() const {
	return bufSize;
}

bool OFile::hasOpenedTheFile() const {
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
