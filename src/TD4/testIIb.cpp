#include <iostream>

#include "semaphore.hpp"
#include "thread.hpp"

void thread_print(size_t i, Semaphore& waitingSemaphore, Semaphore& releasingSemaphore) {
	for(int j = 0; j < 10; j++) {
		waitingSemaphore.wait();
		std::cout << "Affichage " << j + 1 << " du thread " << i << std::endl;
		releasingSemaphore.post();
	}
}

int main() {
	Semaphore sem[3];

	for(int i = 0; i < 3; ++i) {
		sem[i] = Semaphore(0);
	}
	sem[0].post();

	Thread threads[3];

	for(size_t i = 1; i <= 3; ++i) {
		int const waitingSemaphoreID = i - 1, releasingSemaphoreID = i % 3;
		threads[i - 1].start(
		  thread_print, i, std::ref(sem[waitingSemaphoreID]), std::ref(sem[releasingSemaphoreID]));
	}

	for(int i = 0; i < 3; ++i) {
		threads[i].join();
	}
}
