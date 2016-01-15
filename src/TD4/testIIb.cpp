#include <iostream>

#include <pthread.h>

#include "semaphore.hpp"

Semaphore sem[3];

void* thread_print(void* thread_id) {

	long int i = reinterpret_cast<long int>(thread_id);

	int const waitingSemaphoreID = i - 1, releasingSemaphoreID = i % 3;

	for(int j = 0; j < 10; j++) {
		sem[waitingSemaphoreID].wait();
		std::cout << "Affichage " << j + 1 << " du thread " << i << std::endl;
		sem[releasingSemaphoreID].post();
	}

	pthread_exit(nullptr);
}

int main() {
	for(int i = 0; i < 3; ++i) {
		sem[i] = Semaphore(0);
	}
	sem[0].post();

	pthread_t threadIds[3];

	for(long int i = 1; i <= 3; ++i) {
		pthread_create(&threadIds[i - 1], nullptr, thread_print, reinterpret_cast<void*>(i));
	}

	for(int i = 0; i < 3; ++i) {
		pthread_join(threadIds[i], nullptr);
	}
}
