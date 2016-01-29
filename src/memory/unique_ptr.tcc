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
UniquePtr<T, Deleter>::UniquePtr(UniquePtr&& other) noexcept
      : pointer(std::move(other.pointer)),
        deleter(std::move(deleter)) {
	other.release();
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr<T, Deleter>&& other) noexcept {
	if(&other != this) {
		this->pointer = std::move(other.pointer);
		this->deleter = std::move(other.deleter);
		other.release();
	}
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::~UniquePtr() {
	if(pointer != nullptr) {
		deleter(pointer);
	}
}

template <typename T, typename Deleter>
T* UniquePtr<T, Deleter>::get() {
	return pointer;
}

template <typename T, typename Deleter>
T* UniquePtr<T, Deleter>::release() {
	T* pointer = this->pointer;
	this->pointer = nullptr;
	return pointer;
}

template <typename T, typename Deleter>
Deleter UniquePtr<T, Deleter>::getDeleter() const {
	return deleter;
}
