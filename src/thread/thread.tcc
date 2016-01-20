#pragma once

#include "thread.hpp"

#include <functional>
#include <memory>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>

#include <cstddef>

#include <pthread.h>

#include "invoke.hpp"

template <typename Functor, typename... Arguments>
void Thread::start(Functor&& threadMainFunction, Arguments&&... args) noexcept(false) {
	static_assert(std::is_convertible<Functor, std::function<void(Arguments...)>>::value,
			"The function must be convertible to a function of type void(Arguments...)");

	auto threadData =
	  std::make_unique<std::tuple<std::decay_t<Functor>, std::decay_t<Arguments>...>>(
	    std::forward<Functor>(threadMainFunction), std::forward<Arguments>(args)...);

	joinable = true;
	int rc = pthread_create(&thread, nullptr,
	                        makeWrapperFunction<std::decay_t<Functor>, std::decay_t<Arguments>...>(
	                          std::index_sequence_for<Functor, Arguments...>()),
	                        threadData.get());

	if(rc == 0) {
		threadData.release();
	} else {
		throw std::system_error(rc, std::system_category());
	}
}

template <typename Functor, typename... Arguments, size_t... I>
constexpr auto Thread::makeWrapperFunction(std::index_sequence<0, I...>) {
	return [](void* voidData) -> void* {

		auto dataPointer = std::unique_ptr<std::tuple<Functor, Arguments...>>(
		  static_cast<std::tuple<Functor, Arguments...>*>(voidData));

		// Call the function (at index 0), with the arguments as parameters (index 1, 2, 3, ...)
		//
		// C++17: permit the call of member function (see source)
		invoke(std::move(std::get<0>(*dataPointer)),
			   std::move(std::get<I>(*dataPointer))...);
		// Alternative:
		//std::move(std::get<0>(*dataPointer))(std::move(std::get<I>(*dataPointer))...);

		pthread_exit(nullptr);
	};
}
