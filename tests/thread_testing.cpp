#include "thread.hpp"

#include <memory>
#include <mutex>
#include <sstream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(thread_construction) {
	Thread myThread;

	BOOST_CHECK(!myThread.isJoinable());
}

BOOST_AUTO_TEST_CASE(thread_start) {
	Thread myThread, myOtherThread;

	myThread.start([]() {});
	myOtherThread.start([]() {});

	BOOST_CHECK(myThread.isJoinable());
	BOOST_CHECK(myOtherThread.isJoinable());

	// With at least g++, a POSIX thread is actually an unsigned int (which is actully just a
	// pointer) so you can compare them.
	BOOST_CHECK_NE(myThread.getPosixThread(), myOtherThread.getPosixThread());

	myThread.join();
	myOtherThread.join();
}

typedef struct {
	short i;
} A;
class B {};
union C {};

A myA = {42};
B myB;
C myC;

BOOST_AUTO_TEST_CASE(thread_variadic_start) {
	Thread myThread;

	A otherA = {42};

	myThread
	  .start([](A a, A const ac, A& ar, A const& acr, A&& am, A* ap, A const* acp, B b, C c, int i, auto y) {
		  BOOST_CHECK_NE(&a, &myA);
		  BOOST_CHECK_EQUAL(a.i, 42);
		  BOOST_CHECK_NE(&ac, &myA);
		  BOOST_CHECK_EQUAL(&ar, &myA);
		  BOOST_CHECK_EQUAL(&acr, &myA);
		  BOOST_CHECK_EQUAL(am.i, 42);
		  BOOST_CHECK_EQUAL(ap, &myA);
		  BOOST_CHECK_EQUAL(acp, &myA);
		  BOOST_CHECK((std::is_same<B, decltype(b)>::value));
		  BOOST_CHECK((std::is_same<C, decltype(c)>::value));
		  BOOST_CHECK_EQUAL(i, 42);
		  BOOST_CHECK_EQUAL(y, 'y');
	  }, myA, myA, std::ref(myA), std::ref(myA), std::move(otherA), &myA, &myA, myB, myC, 42, 'y');

	myThread.join();
}

BOOST_AUTO_TEST_CASE(thread_start_lambda_capture) {
	Thread myThread;

	A a{42}, ab{1337}, abc{69};

	myThread.start([a, &ab, abc = std::move(abc)](A* ap, A* abp) {
		BOOST_CHECK_NE(&a, ap);
		BOOST_CHECK_EQUAL(a.i, 42);

		BOOST_CHECK_EQUAL(&ab, abp);
		BOOST_CHECK_EQUAL(ab.i, 1337);

		BOOST_CHECK_EQUAL(abc.i, 69);
	}, &a, &ab);

	myThread.join();
}

BOOST_AUTO_TEST_CASE(thread_join) {
	Thread myThread;

	bool threadExecuted = false;
	myThread.start([&threadExecuted]() { threadExecuted = true; });
	myThread.join();
	BOOST_CHECK(threadExecuted);
}

void thread_detach_impl(std::shared_ptr<std::mutex> mutexP, size_t& count) {
	std::lock_guard<std::mutex> lock(*mutexP);

	auto threadExecuted = std::make_shared<bool>(false);

	Thread myThread;

	myThread.start([threadExecuted, &count, mutex = mutexP]() {
		std::lock_guard<std::mutex> lock(*mutex);
		*threadExecuted = true;
		count = threadExecuted.use_count();
	});

	BOOST_CHECK(!*threadExecuted);

	myThread.detach();
}

BOOST_AUTO_TEST_CASE(thread_detach) {
	auto mutex = std::make_shared<std::mutex>();

	size_t use_count;

	thread_detach_impl(mutex, use_count);

	std::lock_guard<std::mutex> lock(*mutex);

	//BOOST_CHECK_EQUAL(use_count, 2);
}
