#pragma once

template <typename T, typename Deleter>
class UniquePtr {
public:
	explicit UniquePtr(T* pointer);

	template <typename D>
	UniquePtr(T* pointer, D&& deleter);

	~UniquePtr();

	T* get();

protected:
	T* pointer;
	Deleter deleter;
};

#include "unique_ptr.tcc"
