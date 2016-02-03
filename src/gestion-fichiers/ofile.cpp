#include "ofile.hpp"

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
