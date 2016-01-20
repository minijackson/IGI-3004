#include "thread.hpp"

#include <exception>
#include <system_error>

Thread::Thread()
      : thread() {}

Thread::~Thread() {
	if(joinable) {
		std::terminate();
	}
}

void Thread::detach() noexcept(false) {
	int rc = pthread_detach(thread);
	if(rc != 0) {
		throw std::system_error(rc, std::system_category());
	}
	joinable = false;
}

void Thread::join() noexcept(false) {
	int rc = pthread_join(thread, nullptr);
	if(rc != 0) {
		throw std::system_error(rc, std::system_category());
	}
	joinable = false;
}

pthread_t Thread::getPosixThread() const {
	return thread;
}

bool Thread::isJoinable() const {
	return joinable;
}
