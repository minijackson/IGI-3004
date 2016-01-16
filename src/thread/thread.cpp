#include "thread.hpp"

Thread::Thread()
      : thread() {}

Thread::~Thread() {
	if(joinable) {
		std::terminate();
	}
}

void Thread::detach() throw(std::system_error) {
	int rc = pthread_detach(thread);
	if(rc != 0) {
		throw std::system_error(rc, std::system_category());
	}
	joinable = false;
}

void Thread::join() throw(std::system_error) {
	int rc = pthread_join(thread, nullptr);
	if(rc != 0) {
		throw std::system_error(rc, std::system_category());
	}
	joinable = false;
}
