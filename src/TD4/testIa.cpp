#include <iostream>
#include <memory>

#include <cstring>

#include <pthread.h>

#include "gestion-fichiers.hpp"
#include "semaphore.hpp"
#include "thread.hpp"

constexpr size_t const TAILLEBUF = 2048;

void readAndStore(IFile& source, char* data) {
	char line[TAILLEBUF];
	source >> line;
	std::cout << "[Reading]: Line read." << std::endl;

	std::strncpy(data, line, TAILLEBUF);
}

void readingThreadMain(std::string filename,
                       char data[],
                       Semaphore& readingLock,
                       Semaphore& writingLock) {

	std::cout << "[Reading]: Thread spawned." << std::endl;

	IFile source(filename.c_str(), TAILLEBUF);

	while(!source.hasEnded()) {
		readingLock.wait();
		readAndStore(source, data);
		writingLock.post();
	}

	std::cout << "[Reading]: Sending End-of-Transmission" << std::endl;
	data[0] = 0x04;
	writingLock.post();

	std::cout << "[Reading]: Goodbye!" << std::endl;
	pthread_exit(NULL);
}

void getAndTransmit(OFile& destination, char* data) {
	destination << data;
}

void transmittingThreadMain(std::string filename,
                            char data[],
                            Semaphore& readingLock,
                            Semaphore& writingLock) {

	std::cout << "[Transmission]: Thread spawned." << std::endl;

	OFile destination(filename.c_str(), TAILLEBUF);

	writingLock.wait();

	while(data[0] != 0x04) {
		destination << data;
		readingLock.post();
		std::cout << "[Transmission]: Line transmitted." << std::endl;

		writingLock.wait();
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

	auto readingLock = Semaphore(1), writingLock = Semaphore(0), readingReady = Semaphore(0),
	     transmissionReady = Semaphore(0);

	auto c = std::make_unique<char[]>(TAILLEBUF);
	c.get()[0] = '\0';

	Thread transmittingThread, readingThread;

	transmittingThread.start(transmittingThreadMain,
	                         std::move(argv[2]),
	                         c.get(),
	                         std::ref(readingLock),
	                         std::ref(writingLock));

	readingThread.start(
	  readingThreadMain, std::move(argv[1]), c.get(), std::ref(readingLock), std::ref(writingLock));

	std::cout << "[Main]: Waiting for Reading and Transmission to finish" << std::endl;
	transmittingThread.join();
	readingThread.join();

	std::cout << "[Main]: Goodbye!" << std::endl;
	pthread_exit(NULL);
}
