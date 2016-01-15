#include <iostream>
#include <memory>

#include <cstring>

#include <pthread.h>
#include <semaphore.h>

#include "gestion-fichiers.hpp"
#include "semaphore.hpp"

constexpr size_t const TAILLEBUF = 2048;

struct ReadingThreadData {
	std::string filename;
	std::shared_ptr<char> data;
	std::shared_ptr<Semaphore> readingLock;
	std::shared_ptr<Semaphore> writingLock;
	Semaphore* ready;
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

	std::shared_ptr<Semaphore> readingLock = threadData->readingLock;
	std::shared_ptr<Semaphore> writingLock = threadData->writingLock;
	std::shared_ptr<char> data             = threadData->data;

	IFile source(threadData->filename.c_str(), TAILLEBUF);

	std::cout << "[Reading]: Ready!" << std::endl;
	threadData->ready->post();

	while(!source.hasEnded()) {
		readingLock->wait();
		readAndStore(source, data.get());
		writingLock->post();
	}

	std::cout << "[Reading]: Sending End-of-Transmission" << std::endl;
	data.get()[0] = 0x04;
	writingLock->post();

	std::cout << "[Reading]: Goodbye!" << std::endl;
	pthread_exit(NULL);
}

struct TransmittingThreadData {
	std::string filename;
	std::shared_ptr<char> data;
	std::shared_ptr<Semaphore> readingLock;
	std::shared_ptr<Semaphore> writingLock;
	Semaphore* ready;
};

void getAndTransmit(OFile& destination, char* data) {
	destination << data;
}

void* transmittingThreadMain(void* voidThreadData) {
	std::cout << "[Transmission]: Thread spawned." << std::endl;

	struct TransmittingThreadData* threadData =
	  reinterpret_cast<struct TransmittingThreadData*>(voidThreadData);

	std::shared_ptr<Semaphore> readingLock = threadData->readingLock;
	std::shared_ptr<Semaphore> writingLock = threadData->writingLock;
	std::shared_ptr<char> data             = threadData->data;

	OFile destination(threadData->filename.c_str(), TAILLEBUF);

	std::cout << "[Transmission]: Ready!" << std::endl;
	threadData->ready->post();

	writingLock->wait();

	while(data.get()[0] != 0x04) {
		destination << data.get();
		readingLock->post();
		std::cout << "[Transmission]: Line transmitted." << std::endl;

		writingLock->wait();
	}
	std::cout << "[Transmission]: Received End-of-Transmission" << std::endl;

	std::cout << "[Transmission]: Goodbye!" << std::endl;
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " source destination" << std::endl;
		return EINVAL;
	}

	std::shared_ptr<Semaphore> readingLock(new Semaphore(1));
	std::shared_ptr<Semaphore> writingLock(new Semaphore(0));
	std::unique_ptr<Semaphore> readingReady(new Semaphore(0));
	std::unique_ptr<Semaphore> transmissionReady(new Semaphore(0));

	pthread_t transmittingThread, readingThread;

	std::shared_ptr<char> c(new char[TAILLEBUF], std::default_delete<char[]>());
	c.get()[0] = '\0';

	std::unique_ptr<struct TransmittingThreadData> transmittingThreadData(
	  new struct TransmittingThreadData);
	transmittingThreadData->filename    = argv[2];
	transmittingThreadData->data        = c;
	transmittingThreadData->readingLock = readingLock;
	transmittingThreadData->writingLock = writingLock;
	transmittingThreadData->ready       = readingReady.get();

	std::unique_ptr<struct ReadingThreadData> readingThreadData(new struct ReadingThreadData);
	readingThreadData->filename    = argv[1];
	readingThreadData->data        = std::move(c);
	readingThreadData->readingLock = readingLock;
	readingThreadData->writingLock = writingLock;
	readingThreadData->ready       = transmissionReady.get();

	pthread_create(&transmittingThread, NULL, transmittingThreadMain,
	               reinterpret_cast<void*>(transmittingThreadData.get()));
	pthread_create(&readingThread, NULL, readingThreadMain,
	               reinterpret_cast<void*>(readingThreadData.get()));

	pthread_detach(transmittingThread);
	pthread_detach(readingThread);

	std::cout << "[Main]: Waiting for Reading and Transmission to be Ready" << std::endl;
	readingReady->wait();
	transmissionReady->wait();

	std::cout << "[Main]: Goodbye!" << std::endl;
	pthread_exit(NULL);
}
