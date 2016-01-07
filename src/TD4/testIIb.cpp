#include <iostream>

#include <pthread.h>
#include <semaphore.h>

sem_t sem[3];

void* thread_print(void* thread_id) {

	long int i = reinterpret_cast<long int>(thread_id);

	int const waitingSemaphoreID = i - 1, releasingSemaphoreID = i % 3;

	for(int j = 0; j < 10; j++) {
		sem_wait(&sem[waitingSemaphoreID]);
		std::cout << "Affichage " << j + 1 << " du thread " << i << std::endl;
		sem_post(&sem[releasingSemaphoreID]);
	}

	pthread_exit(nullptr);
}

int main() {
	for(int i = 0; i < 3; ++i) {
		sem_init(&sem[i], /* __pshared = */ 0, /* __value = */ 1);
		if(i != 0) {
			sem_wait(&sem[i]);
		}
	}

	pthread_t threadIds[3];

	for(long int i = 1; i <= 3; ++i) {
		pthread_create(&threadIds[i - 1], nullptr, thread_print, reinterpret_cast<void*>(i));
	}

	for(int i = 0; i < 3; ++i) {
		pthread_join(threadIds[i], nullptr);
	}
}
