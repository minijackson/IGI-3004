#include "ofile.hpp"

template <>
OFile<unbuffered_flag>::~OFile() {
	close();
}

template <>
OFile<buffered_flag>::~OFile() {
	if(!dummy) {
		if(write(fd, buffer.str().c_str(), buffer.str().size()) == -1) {
			throw std::ios_base::failure("Cannot write in file",
			                             std::error_code(errno, std::system_category()));
		}
		close();
	}
}

template <>
void OFile<unbuffered_flag>::writeLine(char const* content) {
	size_t len = std::strlen(content);
	if(len > bufSize) {
		len = bufSize;
	}

	if(write(fd, content, len) == -1) {
		throw std::ios_base::failure("Cannot write in file",
		                             std::error_code(errno, std::system_category()));
	}
}

template <>
void OFile<buffered_flag>::writeLine(char const* content) {
	buffer << content;

	if(buffer.str().size() > bufSize) {
		if(write(fd, buffer.str().c_str(), buffer.str().size()) == -1) {
			throw std::ios_base::failure("Cannot write in file",
			                             std::error_code(errno, std::system_category()));
		} else {
			buffer.str("");
		}
	}
}
