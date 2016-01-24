#pragma once

#include "unique_ptr.hpp"

#include <utility>

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr(T* pointer)
      : pointer(pointer) {}

template <typename T, typename Deleter>
template <typename D>
UniquePtr<T, Deleter>::UniquePtr(T* pointer, D&& deleter)
      : pointer(pointer)
      , deleter(std::forward<Deleter>(deleter)) {}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::~UniquePtr() {
	deleter(pointer);
}

template <typename T, typename Deleter>
T* UniquePtr<T, Deleter>::get() {
	return pointer;
}
