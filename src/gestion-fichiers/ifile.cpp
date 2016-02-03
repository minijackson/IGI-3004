#include "ifile.hpp"

#include <memory>

template <>
IFile<unbuffered_flag>::IFile(IFile&& other) noexcept
      : dummy(other.dummy),
        fd(other.fd),
        bufSize(other.bufSize),
        ended(other.ended),
        openedTheFile(other.openedTheFile) {
	other.dummy         = true;
	other.openedTheFile = false;
}

template <>
IFile<buffered_flag>::IFile(IFile&& other) noexcept
      : DummyFile(std::move(other)),
        dummy(other.dummy),
        fd(other.fd),
        bufSize(other.bufSize),
        ended(other.ended),
        openedTheFile(other.openedTheFile) {
	other.dummy         = true;
	other.openedTheFile = false;
}

template <>
IFile<unbuffered_flag>& IFile<unbuffered_flag>::operator=(IFile&& other) noexcept {

	this->dummy         = other.dummy;
	this->fd            = other.fd;
	this->bufSize       = other.bufSize;
	this->ended         = other.ended;
	this->openedTheFile = other.openedTheFile;

	other.dummy         = true;
	other.openedTheFile = false;
	return *this;
}

template <>
IFile<buffered_flag>& IFile<buffered_flag>::operator=(IFile&& other) noexcept {

	this->buffer        = std::move(other.buffer);
	this->dummy         = other.dummy;
	this->fd            = other.fd;
	this->bufSize       = other.bufSize;
	this->ended         = other.ended;
	this->openedTheFile = other.openedTheFile;

	other.dummy         = true;
	other.openedTheFile = false;
	return *this;
}

template <>
void IFile<unbuffered_flag>::getLine(char* buf) {
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

template <>
void IFile<buffered_flag>::getLine(char* buf) {
	if(!dummy) {
		// If there is no more characters to extract from the string stream
		if(buffer.rdbuf()->in_avail() == 0) {
			std::unique_ptr<char[]> tempBuf = std::make_unique<char[]>(bufSize);
			ssize_t status = read(fd, tempBuf.get(), bufSize);
			if(status == 0) {
				ended = true;
			} else if(status > 0) {
				buffer.str(tempBuf.get());
			} else {
				throw std::system_error(errno, std::system_category());
			}
		}

		buffer.getline(buf, bufSize - 1);
		if(!ended) {
			std::strncat(buf, "\n", bufSize);
		}
	}
}
