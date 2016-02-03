#pragma once

#include "ofile.hpp"

#include <ios>
#include <system_error>

#include <cerrno>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

template <typename flag>
OFile<flag>::OFile() noexcept
      : dummy(true)
      , openedTheFile(false) {}

template <typename flag>
OFile<flag>::OFile(int const fd, size_t const bufSize) noexcept
      : fd(fd)
      , bufSize(bufSize) {}

template <typename flag>
OFile<flag>::OFile(char const* filename, size_t const bufSize)
      : fd(openFile(filename))
      , bufSize(bufSize)
      , openedTheFile(true) {}

template <typename flag>
OFile<flag>::~OFile() {
	close();
}

template <typename flag>
OFile<flag>::OFile(OFile&& other) noexcept
      : dummy(other.dummy),
        fd(other.fd),
        bufSize(other.bufSize),
        openedTheFile(other.openedTheFile) {
	other.dummy         = true;
	other.openedTheFile = false;
}

template <typename flag>
OFile<flag>& OFile<flag>::operator=(OFile&& other) noexcept {
	this->dummy = other.dummy;
	this->fd = other.fd;
	this->bufSize = other.bufSize;
	this->openedTheFile = other.openedTheFile;

	other.dummy         = true;
	other.openedTheFile = false;
	return *this;
}

template <typename flag>
OFile<flag>& OFile<flag>::operator<<(char const* line) {
	writeLine(line);
	return *this;
}

template <typename flag>
void OFile<flag>::close() {
	if(openedTheFile) {
		::close(fd);
	}
	dummy = true;
}

template <typename flag>
int OFile<flag>::getFd() const {
	return fd;
}

template <typename flag>
size_t OFile<flag>::getBufSize() const {
	return bufSize;
}

template <typename flag>
bool OFile<flag>::hasOpenedTheFile() const {
	return openedTheFile;
}

template <typename flag>
int OFile<flag>::openFile(char const* filename) {
	int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if(fd == -1) {
		throw std::ios_base::failure("Cannot open file for writing",
		                             std::error_code(errno, std::system_category()));
	}
	return fd;
}
