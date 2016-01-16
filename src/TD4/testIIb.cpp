#include <iostream>

#include "semaphore.hpp"
#include "thread.hpp"

Semaphore sem[3];

void thread_print(size_t i) {
	int const waitingSemaphoreID = i - 1, releasingSemaphoreID = i % 3;

	for(int j = 0; j < 10; j++) {
		sem[waitingSemaphoreID].wait();
		std::cout << "Affichage " << j + 1 << " du thread " << i << std::endl;
		sem[releasingSemaphoreID].post();
	}
}

int main() {
	for(int i = 0; i < 3; ++i) {
		sem[i] = Semaphore(0);
	}
	sem[0].post();

	Thread threads[3];

	for(size_t i = 1; i <= 3; ++i) {
		threads[i - 1].start(thread_print, i);
	}

	for(int i = 0; i < 3; ++i) {
		threads[i].join();
	}
}
