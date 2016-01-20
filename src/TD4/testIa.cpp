#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <cerrno>
#include <cstring>

#include <pthread.h>

#include "gestion-fichiers.hpp"
#include "semaphore.hpp"
#include "thread.hpp"

constexpr size_t const TAILLEBUF = 2048;

/*=====================*/
/*== Prototypes: {{{ ==*/
/*=====================*/
void readAndStore(IFile& source, char* data);
void readingThreadMain(std::string filename,
                       char data[],
                       Semaphore& readingLock,
                       Semaphore& writingLock,
                       bool& readingFinished);

void getAndTransmit(OFile& destination, char* data);
void transmittingThreadMain(std::string filename,
                            char data[],
                            Semaphore& readingLock,
                            Semaphore& writingLock,
                            bool& readingFinished);
// }}}

void readAndStore(IFile& source, char* data) {
	char line[TAILLEBUF];
	source >> line;
	std::cout << "[Reading]: Line read." << std::endl;

	std::strncpy(data, line, TAILLEBUF);
}

void readingThreadMain(std::string filename,
                       char data[],
                       Semaphore& readingLock,
                       Semaphore& writingLock,
                       bool& readingFinished) {

	std::cout << "[Reading]: Thread spawned." << std::endl;

	IFile source(filename.c_str(), TAILLEBUF);

	while(!source.hasEnded()) {
		readingLock.wait();
		readAndStore(source, data);
		writingLock.post();
	}

	std::cout << "[Reading]: Finished!" << std::endl;
	readingFinished = true;
	writingLock.post();

	std::cout << "[Reading]: Goodbye!" << std::endl;
}

void getAndTransmit(OFile& destination, char* data) {
	destination << data;
}

void transmittingThreadMain(std::string filename,
                            char data[],
                            Semaphore& readingLock,
                            Semaphore& writingLock,
                            bool& readingFinished) {

	std::cout << "[Transmission]: Thread spawned." << std::endl;

	OFile destination(filename.c_str(), TAILLEBUF);

	writingLock.wait();

	while(!readingFinished) {
		destination << data;
		readingLock.post();
		std::cout << "[Transmission]: Line transmitted." << std::endl;

		writingLock.wait();
	}
	std::cout << "[Transmission]: Reading finished!" << std::endl;

	std::cout << "[Transmission]: Goodbye!" << std::endl;
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " source destination" << std::endl;
		return EINVAL;
	}

	auto readingLock = Semaphore(1), writingLock = Semaphore(0);

	bool readingFinished = false;

	auto c = std::make_unique<char[]>(TAILLEBUF);
	c.get()[0] = '\0';

	Thread transmittingThread, readingThread;

	transmittingThread.start(transmittingThreadMain,
	                         std::move(argv[2]),
	                         c.get(),
	                         std::ref(readingLock),
	                         std::ref(writingLock),
	                         std::ref(readingFinished));

	readingThread.start(readingThreadMain,
	                    std::move(argv[1]),
	                    c.get(),
	                    std::ref(readingLock),
	                    std::ref(writingLock),
	                    std::ref(readingFinished));

	std::cout << "[Main]: Waiting for Reading and Transmission to finish" << std::endl;
	transmittingThread.join();
	readingThread.join();

	std::cout << "[Main]: Goodbye!" << std::endl;
	pthread_exit(NULL);
}
