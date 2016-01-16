#pragma once

#include <memory>
#include <tuple>

#include "thread.hpp"

template <typename... Arguments>
void Thread::start(void (*threadMainFunction)(Arguments...),
                   Arguments... args) throw(std::system_error) {

	auto threadData = std::make_unique<std::tuple<void (*)(Arguments...), Arguments...>>(
	  threadMainFunction, std::forward<Arguments...>(args...));

	wrapperFunction = makeWrapperFunction<Arguments...>(
	  std::index_sequence_for<void (*)(Arguments...), Arguments...>()),

	joinable = true;
	int rc = pthread_create(&thread, nullptr, wrapperFunction, threadData.get());
	if(rc == 0) {
		threadData.release();
	} else {
		throw std::system_error(rc, std::system_category());
	}
}

template <typename... Arguments, size_t... I>
constexpr auto Thread::makeWrapperFunction(std::index_sequence<0, I...>) {
	return [](void* voidData) -> void* {

		auto dataPointer = std::unique_ptr<std::tuple<void (*)(Arguments...), Arguments...>>(
		  static_cast<std::tuple<void (*)(Arguments...), Arguments...>*>(voidData));
		// Call the function (at index 0), with the arguments as parameters (index 1, 2, 3, ...)
		std::get<0>(*dataPointer)(std::forward<Arguments...>(std::get<I>(*dataPointer)...));

		pthread_exit(nullptr);
	};
}
