#pragma once

#include "unique_ptr.hpp"

#include <memory>
#include <type_traits>
#include <vector>

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
	      : dimensions({dimensions...})
	      , dimensionId(sizeof...(Dimensions)) {}

	template <typename T>
	auto operator()(T* pointer)
	  -> std::enable_if_t<std::is_pointer<std::decay_t<T>>::value && !std::is_class<T>::value> {
		--dimensionId;
		for(size_t i = 0; i < dimensions[dimensionId]; ++i) {
			(*this)(pointer[i]);
			free(static_cast<void*>(pointer[i]));
		}
		free(static_cast<void*>(pointer));
	}

	template <typename T>
	auto operator()(T* pointer)
	  -> std::enable_if_t<std::is_pointer<std::decay_t<T>>::value && std::is_class<T>::value> {
		--dimensionId;
		for(size_t i = 0; i < dimensions[dimensionId]; ++i) {
			pointer[i].~T();
			free(static_cast<void*>(pointer[i]));
		}
		free(static_cast<void*>(pointer));
	}

	template <typename T>
	auto operator()(T*) -> std::enable_if_t<!std::is_pointer<std::decay_t<T>>::value> {}

protected:
	std::vector<size_t> dimensions;
	size_t dimensionId;
};

namespace detail {

	template <typename T, typename... Dimensions>
	auto makeMultiDimUniquePtr_impl(T&, Dimensions...)
	  -> std::enable_if_t<!std::is_pointer<std::decay_t<T>>::value> {}

	template <typename T, typename Size, typename... Dimensions>
	auto makeMultiDimUniquePtr_impl(T& tab, Size dim, Dimensions... otherDims)
	  -> std::enable_if_t<std::is_pointer<std::decay_t<T>>::value> {

		using SubT = std::remove_pointer_t<T>;

		tab = static_cast<T>(std::malloc(dim * sizeof(SubT)));
		for(size_t i = 0; i < dim; ++i) {
			makeMultiDimUniquePtr_impl(tab[i], otherDims...);
		}
	}
}

template <typename T, typename... Arguments>
inline UniquePtr<T, std::default_delete<T>> makeUniquePtr(Arguments&&... value) {
	return UniquePtr<T, std::default_delete<T>>(new T(std::forward<Arguments>(value)...));
}

template <typename T>
inline UniquePtr<T, std::default_delete<T>> makeMallocUniquePtr() {
	return UniquePtr<T, FreeDeleter<T>>(static_cast<T*>(std::malloc(sizeof(T))));
}

template <typename T, typename... Arguments>
inline UniquePtr<T, std::default_delete<T>> makeMallocUniquePtr(Arguments&&... value) {
	T* ptr = static_cast<T*>(std::malloc(sizeof(T)));
	*ptr   = T(std::forward<Arguments>(value)...);

	return UniquePtr<T, FreeDeleter<T>>(new T(std::forward<Arguments>(value)...));
}

template <typename T>
inline UniquePtr<T, std::default_delete<T>> makeCallocUniquePtr() {
	return UniquePtr<T, FreeDeleter<T>>(static_cast<T*>(std::calloc(1, sizeof(T))));
}

template <typename T, typename... Dimensions>
inline UniquePtr<std::remove_pointer_t<T>, MultiDimDeleter> makeMultiDimUniquePtr(
  Dimensions... dim) {
	T tab;
	detail::makeMultiDimUniquePtr_impl(tab, dim...);
	return UniquePtr<std::remove_pointer_t<T>, MultiDimDeleter>(tab, MultiDimDeleter(dim...));
}
