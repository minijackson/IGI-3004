#include "semaphore.hpp"

Semaphore::Semaphore() throw(std::system_error)
      : semaphore() {
	if(sem_init(&semaphore, /* __pshared = */ 0, /* __value = */ 1) == -1) {
		throw std::system_error(errno, std::system_category());
	}
}

Semaphore::Semaphore(unsigned int value) throw(std::system_error)
      : semaphore() {
	if(sem_init(&semaphore, /* __pshared = */ 0, value) == -1) {
		throw std::system_error(errno, std::system_category());
	}
}

Semaphore::~Semaphore() {
	sem_destroy(&semaphore);
}

void Semaphore::wait() throw(std::system_error) {
	if(sem_wait(&semaphore) == -1) {
		throw std::system_error(errno, std::system_category());
	}
}

void Semaphore::post() throw(std::system_error) {
	if(sem_post(&semaphore) == -1) {
		throw std::system_error(errno, std::system_category());
	}
}

int Semaphore::getValue() throw(std::system_error) {
	int value;

	if(sem_getvalue(&semaphore, &value) == -1) {
		throw std::system_error(errno, std::system_category());
	}

	return value;
}
