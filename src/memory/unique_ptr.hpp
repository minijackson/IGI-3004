#pragma once

template <typename T, typename Deleter>
class UniquePtr {
public:
	explicit UniquePtr(T* pointer);

	template <typename D>
	UniquePtr(T* pointer, D&& deleter);

	UniquePtr(const UniquePtr&) = delete;
	UniquePtr& operator=(const UniquePtr&) = delete;

	UniquePtr(UniquePtr&&) noexcept;
	UniquePtr& operator=(UniquePtr&&) noexcept;

	~UniquePtr();

	T* get();
	T* release();
	Deleter getDeleter() const;

protected:
	T* pointer;
	Deleter deleter;
};

#include "unique_ptr.tcc"
