#pragma once

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
	auto operator()(T* pointer) -> std::enable_if_t<std::is_pointer<std::decay_t<T>>::value> {
		static size_t dimensionId = dimensionCount;

		--dimensionId;
		for(size_t i = 0; i < dimensions[dimensionId]; ++i) {
			(*this)(pointer[i]);
			free(static_cast<void*>(pointer[i]));
		}
		free(static_cast<void*>(pointer));
	}

	template <typename T>
	auto operator()(T*) -> std::enable_if_t<!std::is_pointer<std::decay_t<T>>::value> {}

protected:
	size_t dimensionCount;
	std::vector<size_t> dimensions;
};

namespace detail {

	template <typename T, typename... Dimensions>
	auto makeMultiDimUniquePtr_impl(T&, Dimensions...)
	  -> std::enable_if_t<!std::is_pointer<std::decay_t<T>>::value> {}

	template <typename T, typename Size, typename... Dimensions>
	auto makeMultiDimUniquePtr_impl(T& tab, Size dim, Dimensions... otherDims)
	  -> std::enable_if_t<std::is_pointer<std::decay_t<T>>::value> {

		using SubT = std::remove_pointer_t<T>;

		tab = static_cast<T>(malloc(dim * sizeof(SubT)));
		for(size_t i = 0; i < dim; ++i) {
			makeMultiDimUniquePtr_impl(tab[i], otherDims...);
		}
	}
}

template <typename T, typename... Dimensions>
UniquePtr<std::remove_pointer_t<T>, MultiDimDeleter> makeMultiDimUniquePtr(Dimensions... dim) {
	T tab;
	detail::makeMultiDimUniquePtr_impl(tab, dim...);
	return UniquePtr<std::remove_pointer_t<T>, MultiDimDeleter>(tab, MultiDimDeleter(dim...));
}
