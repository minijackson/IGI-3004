#include <iostream>

#include <cstring>

#include <pthread.h>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 255;

char* c;

void readAndStore(IFile& source) {
	char line[TAILLEBUF];
	source >> line;
	std::cout << "[Reading]: Line read." << std::endl;

	std::strncpy(c, line, TAILLEBUF);
}

void* readingThreadMain(void* filename) {
	std::cout << "[Reading]: Thread spawned." << std::endl;

	IFile source((char*)filename, TAILLEBUF);

	while(!source.hasEnded()) {
		readAndStore(source);
	}

	std::cout << "[Reading]: Sending End-of-Transmission" << std::endl;
	c[0] = 0x04;

	std::cout << "[Reading]: Thread exiting" << std::endl;
	pthread_exit(NULL);
}

void getAndTransmit(OFile& destination) {
	destination << c;
}

void* transmittingThreadMain(void* filename) {
	std::cout << "[Transmission]: Thread spawned." << std::endl;

	OFile destination((char*)filename, TAILLEBUF);

	char previous[TAILLEBUF];

	std::strcpy(previous, c);

	while(c[0] != 0x04) {
		if(std::strcmp(previous, c)) {
			std::strcpy(previous, c);
			destination << previous;
			std::cout << "[Transmission]: Line transmitted." << std::endl;
		}
	}
	std::cout << "[Transmission]: Received End-of-Transmission" << std::endl;

	std::cout << "[Transmission]: Thread exiting" << std::endl;
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " source destination" << std::endl;
		return EINVAL;
	}

	pthread_t threads[2];

	c = new char[TAILLEBUF];
	c[0] = '\0';

	pthread_create(&threads[0], NULL, transmittingThreadMain, static_cast<void*>(argv[2]));
	pthread_create(&threads[1], NULL, readingThreadMain, static_cast<void*>(argv[1]));

	delete c;

	pthread_exit(NULL);
}
