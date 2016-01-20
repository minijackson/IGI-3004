#include "semaphore.hpp"

#include <system_error>

#include <cerrno>

Semaphore::Semaphore(unsigned int value) noexcept(false)
      : semaphore() {
	if(sem_init(&semaphore, /* __pshared = */ 0, value) == -1) {
		throw std::system_error(errno, std::system_category());
	}
}

Semaphore::~Semaphore() {
	sem_destroy(&semaphore);
}

void Semaphore::wait() noexcept(false) {
	if(sem_wait(&semaphore) == -1) {
		throw std::system_error(errno, std::system_category());
	}
}

void Semaphore::post() noexcept(false) {
	if(sem_post(&semaphore) == -1) {
		throw std::system_error(errno, std::system_category());
	}
}

int Semaphore::getValue() noexcept(false) {
	int value;

	if(sem_getvalue(&semaphore, &value) == -1) {
		throw std::system_error(errno, std::system_category());
	}

	return value;
}

sem_t Semaphore::getPosixSemaphore() const {
	return semaphore;
}
