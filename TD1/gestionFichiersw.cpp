#include "gestionFichiersw.hpp"

#include <unistd.h>

int firstline(const int fd, char* buf, const size_t tailleBuf) {
	off_t currentPos = lseek(fd, 0, SEEK_CUR);
	int status = read(fd, buf, tailleBuf);

	if (status == 0) {
		buf[0] = '\0';
		return 0;
	} else if(status != -1) {
		for(size_t i = 0 ; i < tailleBuf ; ++i) {
			if(buf[i] == '\n') {
				lseek(fd, currentPos + ((i+1)*sizeof(char)), SEEK_SET);
				buf[i+1] = '\0';
				return 1;
			}
		}
		return tailleBuf;
	} else {
		return -1;
	}
}
