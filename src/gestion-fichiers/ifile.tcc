#pragma once

#include "gestion-fichiers.hpp"

#include <ios>
#include <system_error>

#include <fcntl.h>
#include <unistd.h>

template <typename flag>
IFile<flag>::IFile() noexcept
      : dummy(true)
      , openedTheFile(false) {}

template <typename flag>
IFile<flag>::IFile(int const fd, size_t const bufSize) noexcept
      : fd(fd)
      , bufSize(bufSize) {}

template <typename flag>
IFile<flag>::IFile(char const* filename, size_t const bufSize)
      : fd(openFile(filename))
      , bufSize(bufSize)
      , openedTheFile(true) {}

template <typename flag>
IFile<flag>::~IFile() {
	close();
}

template <typename flag>
IFile<flag>& IFile<flag>::operator>>(char* line) {
	if(!dummy) {
		getLine(line);
	}
	return *this;
}

template <typename flag>
void IFile<flag>::close() {
	if(openedTheFile) {
		::close(fd);
		openedTheFile = false;
	}
	dummy = true;
}

template <typename flag>
int IFile<flag>::getFd() const {
	return fd;
}

template <typename flag>
size_t IFile<flag>::getBufSize() const {
	return bufSize;
}

template <typename flag>
bool IFile<flag>::hasEnded() const {
	return ended;
}

template <typename flag>
bool IFile<flag>::hasOpenedTheFile() const {
	return openedTheFile;
}

template <typename flag>
int IFile<flag>::openFile(char const* filename) {
	int fd = open(filename, O_RDONLY);
	if(fd == -1) {
		throw std::ios_base::failure("Cannot open file for reading",
		                             std::error_code(errno, std::system_category()));
	}
	return fd;
}
