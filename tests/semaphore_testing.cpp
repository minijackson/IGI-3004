#include "semaphore.hpp"

#include <thread>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(semaphore_creation) {
	Semaphore sem, sem1(/* value = */ 0);

	//BOOST_CHECK_NE(sem.getPosixSemaphore(), sem1.getPosixSemaphore());

	BOOST_CHECK_EQUAL(sem.getValue(), 1);
	BOOST_CHECK_EQUAL(sem1.getValue(), 0);
}

BOOST_AUTO_TEST_CASE(semaphore_copy_construction) {
	Semaphore sem;
	//sem_t referencePosixSemaphore = sem.getPosixSemaphore();

	Semaphore(sem).wait();

	//BOOST_CHECK_EQUAL(referencePosixSemaphore, Semaphore(sem).getPosixSemaphore());
}

BOOST_AUTO_TEST_CASE(semaphore_copy_assignment) {
	Semaphore sem, sem1;
	//sem_t referencePosixSemaphore = sem.getPosixSemaphore();

	sem1 = sem;

	//BOOST_CHECK_EQUAL(referencePosixSemaphore, sem1.getPosixSemaphore());
}

BOOST_AUTO_TEST_CASE(semaphore_move_construction) {
	Semaphore sem;
	//sem_t referencePosixSemaphore = sem.getPosixSemaphore();
	
	Semaphore(std::move(sem)).wait();

	//BOOST_CHECK_EQUAL(referencePosixSemaphore, Semaphore(std::move(sem)).getPosixSemaphore());
}

BOOST_AUTO_TEST_CASE(semaphore_move_assignment) {
	Semaphore sem, sem1;
	//sem_t referencePosixSemaphore = sem.getPosixSemaphore();

	sem1 = std::move(sem);

	//BOOST_CHECK_EQUAL(referencePosixSemaphore, sem1.getPosixSemaphore());
}

BOOST_AUTO_TEST_CASE(semaphore_value) {
	Semaphore sem(0), sem1(1);

	BOOST_CHECK_EQUAL(sem.getValue(), 0);
	BOOST_CHECK_EQUAL(sem1.getValue(), 1);

	sem.post();
	BOOST_CHECK_EQUAL(sem.getValue(), 1);

	sem1.wait();
	BOOST_CHECK_EQUAL(sem1.getValue(), 0);
}

BOOST_AUTO_TEST_CASE(semaphore_wait_post) {
	Semaphore sem(/* value = */ 0);
	bool threadExecuted = false;

	std::thread myThread([&sem, &threadExecuted]() {
		sem.wait();
		threadExecuted = true;
	});

	BOOST_CHECK(!threadExecuted);
	sem.post();
	myThread.join();
	BOOST_CHECK(threadExecuted);
}
