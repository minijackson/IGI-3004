#pragma once

#include <iostream>

#include "unique_ptr.hpp"

#include <vector>
#include <type_traits>

#include <cstdlib>

template <typename T>
struct FreeDeleter_s {
	void operator() (T* pointer) {
		free(static_cast<void*>(pointer));
	}
};

template <typename T>
using FreeDeleter = struct FreeDeleter_s<T>;

class MultiDimDeleter {
public:
	template <typename... Dimensions>
	explicit MultiDimDeleter(Dimensions... dimensions)
	      : dimensionCount(sizeof...(Dimensions))
	      , dimensions({dimensions...}) {}

	template <typename T>
	auto operator()(T* pointer) -> std::enable_if_t<std::is_pointer<decltype(pointer[0])>::value> {
		static size_t dimensionId = dimensionCount;

		--dimensionId;
		for(size_t i = 0; i < dimensions[dimensionId]; ++i) {
			free(static_cast<void*>(pointer[i]));
			(*this)(*pointer);
		}
	}

	template <typename T>
	void operator()(T* pointer) {
		free(static_cast<void*>(pointer));
	}

protected:
	size_t dimensionCount;
	std::vector<size_t> dimensions;
};

// T[1][2][3] = T***
// dimensions = {1, 2, 3}
// => pointer = T*** free(pointer[0], pointer[1], pointer[2])
// => pointer = T**  free(pointer[0], pointer[1])
// => pointer = T*   free(pointer[0])

namespace detail {

	size_t product() {
		return 1;
	}

	template <typename... Rest>
	size_t product(size_t i, Rest... rest) {
		return i * product(rest...);
	}

	template <typename T, typename... Dimensions>
	auto makeMultiDimUniquePtr_impl(T& tab, size_t dim, Dimensions... otherDims)
	  -> std::enable_if_t<std::is_pointer<decltype(tab[0])>::value> {

		std::cout << "Make multi... with dim = " << dim << std::endl;

		using SubT = std::remove_pointer_t<T>;

		tab = static_cast<T>(malloc(detail::product(otherDims...)*sizeof(SubT)));
		for(size_t i = 0; i < dim; ++i) {
			makeMultiDimUniquePtr_impl(tab[i], otherDims...);
		}
	}

	template <typename T, typename... Dimensions>
	void makeMultiDimUniquePtr_impl(T&, Dimensions...) {
		std::cout << "Make multi... catch-all" << std::endl;
	}
}

template <typename T, typename... Dimensions>
UniquePtr<std::remove_pointer_t<T>, MultiDimDeleter> makeMultiDimUniquePtr(Dimensions... dim) {
	T tab;
	detail::makeMultiDimUniquePtr_impl(tab, 0ul, dim...);
	return UniquePtr<std::remove_pointer_t<T>, MultiDimDeleter>(tab, MultiDimDeleter(dim...));
}
