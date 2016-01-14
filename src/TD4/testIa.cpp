#include <iostream>
#include <memory>

#include <cstring>

#include <pthread.h>
#include <semaphore.h>

#include "gestion-fichiers.hpp"

constexpr size_t const TAILLEBUF = 2048;

struct ReadingThreadData {
	std::string filename;
	std::shared_ptr<char> data;
	std::shared_ptr<sem_t> dataLock;
	sem_t* ready;
};

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

	std::shared_ptr<sem_t> dataLock = threadData->dataLock;
	std::shared_ptr<char> data = threadData->data;
	std::cout << "[Reading]: Ready!" << std::endl;
	sem_post(threadData->ready);

	IFile source(threadData->filename.c_str(), TAILLEBUF);

	while(!source.hasEnded()) {
		std::cout << "[Reading] Waiting..." << std::endl;
		sem_wait(dataLock.get());
		readAndStore(source, data.get());
		sem_post(dataLock.get());
	}

	std::cout << "[Reading]: Sending End-of-Transmission" << std::endl;
	data.get()[0] = 0x04;

	std::cout << "[Reading]: Thread exiting" << std::endl;
	pthread_exit(NULL);
}

struct TransmittingThreadData {
	std::string filename;
	std::shared_ptr<char> data;
	std::shared_ptr<sem_t> dataLock;
	sem_t* ready;
};

void getAndTransmit(OFile& destination, char* data) {
	destination << data;
}

void* transmittingThreadMain(void* voidThreadData) {
	std::cout << "[Transmission]: Thread spawned." << std::endl;

	struct TransmittingThreadData* threadData =
	  reinterpret_cast<struct TransmittingThreadData*>(voidThreadData);

	std::shared_ptr<sem_t> dataLock = threadData->dataLock;
	std::shared_ptr<char> data = threadData->data;
	std::cout << "[Transmission]: Ready!" << std::endl;
	sem_post(threadData->ready);

	OFile destination(threadData->filename.c_str(), TAILLEBUF);

	char previous[TAILLEBUF];

	std::strncpy(previous, data.get(), TAILLEBUF);

	while(data.get()[0] != 0x04) {
		if(std::strncmp(previous, data.get(), TAILLEBUF)) {
			std::strncpy(previous, data.get(), TAILLEBUF);
			sem_post(dataLock.get());
			destination << previous;
			std::cout << "[Transmission]: Line transmitted." << std::endl;
		} else {
			sem_post(dataLock.get());
		}
		std::cout << "[Transmission]: Waiting..." << std::endl;
		sem_wait(dataLock.get());
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

	std::shared_ptr<sem_t> dataLock(new sem_t);
	sem_init(dataLock.get(), /* __pshared = */ 0, /* __value = */ 0);

	std::unique_ptr<sem_t> readingReady(new sem_t);
	sem_init(readingReady.get(), /* __pshared = */ 0, /* __value = */ 0);

	std::unique_ptr<sem_t> transmissionReady(new sem_t);
	sem_init(transmissionReady.get(), /* __pshared = */ 0, /* __value = */ 0);

	pthread_t transmittingThread, readingThread;

	std::shared_ptr<char> c(new char[TAILLEBUF], std::default_delete<char[]>());
	c.get()[0] = '\0';

	std::unique_ptr<struct TransmittingThreadData> transmittingThreadData(
	  new struct TransmittingThreadData);
	transmittingThreadData->filename = argv[2];
	transmittingThreadData->data     = c;
	transmittingThreadData->dataLock = dataLock;
	transmittingThreadData->ready    = readingReady.get();

	std::unique_ptr<struct ReadingThreadData> readingThreadData(new struct ReadingThreadData);
	readingThreadData->filename = argv[1];
	readingThreadData->data     = std::move(c);
	readingThreadData->dataLock = dataLock;
	readingThreadData->ready    = transmissionReady.get();

	pthread_create(&transmittingThread, NULL, transmittingThreadMain,
	               reinterpret_cast<void*>(transmittingThreadData.get()));
	pthread_create(&readingThread, NULL, readingThreadMain,
	               reinterpret_cast<void*>(readingThreadData.get()));

	std::cout << "[Main]: Waiting for Reading and Transmission to be Ready" << std::endl;
	sem_wait(readingReady.get());
	sem_wait(transmissionReady.get());

	std::cout << "[Main]: Goodbye!" << std::endl;
	pthread_exit(NULL);
}
