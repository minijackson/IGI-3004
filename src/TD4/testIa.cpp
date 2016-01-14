#include <iostream>
#include <memory>

#include <cstring>

#include <pthread.h>
#include <semaphore.h>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

sem_t dataLock;

struct ReadingThreadData {
	std::string filename;
	char* data;
};

std::unique_ptr<struct ReadingThreadData> readingThreadData(new struct ReadingThreadData);

void readAndStore(IFile& source, char* data) {
	char line[TAILLEBUF];
	source >> line;
	std::cout << "[Reading]: Line read." << std::endl;

	std::strncpy(data, line, TAILLEBUF);
}

void* readingThreadMain(void* voidThreadData) {
	std::cout << "[Reading]: Thread spawned." << std::endl;

	struct ReadingThreadData* threadData =
	  reinterpret_cast<struct ReadingThreadData*>(voidThreadData);

	IFile source(threadData->filename.c_str(), TAILLEBUF);

	while(!source.hasEnded()) {
		std::cout << "[Reading] Waiting..." << std::endl;
		sem_wait(&dataLock);
		readAndStore(source, threadData->data);
		sem_post(&dataLock);
	}

	std::cout << "[Reading]: Sending End-of-Transmission" << std::endl;
	threadData->data[0] = 0x04;

	std::cout << "[Reading]: Thread exiting" << std::endl;
	pthread_exit(NULL);
}

struct TransmittingThreadData {
	std::string filename;
	char* data;
};

std::unique_ptr<struct TransmittingThreadData> transmittingThreadData(
  new struct TransmittingThreadData);

void getAndTransmit(OFile& destination, char* data) {
	destination << data;
}

void* transmittingThreadMain(void* voidThreadData) {
	std::cout << "[Transmission]: Thread spawned." << std::endl;

	struct TransmittingThreadData* threadData =
	  reinterpret_cast<struct TransmittingThreadData*>(voidThreadData);

	OFile destination(threadData->filename.c_str(), TAILLEBUF);

	char previous[TAILLEBUF];

	std::strncpy(previous, threadData->data, TAILLEBUF);

	while(threadData->data[0] != 0x04) {
		if(std::strncmp(previous, threadData->data, TAILLEBUF)) {
			std::strncpy(previous, threadData->data, TAILLEBUF);
			sem_post(&dataLock);
			destination << previous;
			std::cout << "[Transmission]: Line transmitted." << std::endl;
		} else {
			sem_post(&dataLock);
		}
		std::cout << "[Transmission] Waiting..." << std::endl;
		sem_wait(&dataLock);
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

	sem_init(&dataLock, /* __pshared = */ 0, /* __value = */ 0);

	pthread_t transmittingThread, readingThread;

	std::unique_ptr<char[]> c(new char[TAILLEBUF]);
	c.get()[0] = '\0';

	transmittingThreadData->filename = argv[2];
	transmittingThreadData->data = c.get();

	readingThreadData->filename = argv[1];
	readingThreadData->data = c.get();

	pthread_create(&transmittingThread, NULL, transmittingThreadMain,
	               reinterpret_cast<void*>(transmittingThreadData.get()));
	pthread_create(&readingThread, NULL, readingThreadMain,
	               reinterpret_cast<void*>(readingThreadData.get()));

	pthread_join(transmittingThread, nullptr);
	pthread_join(readingThread, nullptr);

	pthread_exit(NULL);
}
