#include "memory.hpp"

#include <memory>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(unique_ptr_construction_destruction) {

	auto lambda = [](int* pointer) {
		BOOST_CHECK_EQUAL(pointer, static_cast<int*>(0x0));
	};

	UniquePtr<int, decltype(lambda)> nullSmartPointer(nullptr, lambda);

	BOOST_CHECK_EQUAL(nullSmartPointer.get(), static_cast<int*>(0x0));
	BOOST_CHECK_EQUAL(nullSmartPointer.getDeleter(), lambda);

	int* a = new int{42};
	UniquePtr<int, std::default_delete<int>> notNullSmartPointer(a);

	BOOST_CHECK_EQUAL(notNullSmartPointer.get(), a);
}

BOOST_AUTO_TEST_CASE(make_unique_ptr) {
	auto smartPtr = makeUniquePtr<int>(42);
	static_assert(
	  std::is_same<decltype(smartPtr), UniquePtr<int, std::default_delete<int>>>::value,
	  "makeUniquePtr<T> did not return an object of type UniquePtr<T, std::default_delete<T>>");

	BOOST_CHECK_NE(smartPtr.get(), static_cast<int*>(0x0));
}
