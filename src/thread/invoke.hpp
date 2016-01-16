#pragma once

#include <utility>
#include <functional>
#include <type_traits>

// C++17 invoke utility
// http://en.cppreference.com/w/cpp/utility/functional/invoke

namespace detail {

	template <class T>
	struct is_reference_wrapper : std::false_type {};
	template <class U>
	struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};

	template <class T>
	constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

	template <class T>
	constexpr bool is_function_v = std::is_function<T>::value;

	template <class Base, class Derived>
	constexpr bool is_base_of_v = std::is_base_of<Base, Derived>::value;

	template <class T>
	constexpr bool is_member_pointer_v = std::is_member_pointer<T>::value;

	template <class Base, class T, class Derived, class... Args>
	inline auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
	  -> std::enable_if_t<is_function_v<T> && is_base_of_v<Base, std::decay_t<Derived>>,
	                      decltype((std::forward<Derived>(ref).*
	                                pmf)(std::forward<Args>(args)...))> {
		return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
	}

	template <class Base, class T, class RefWrap, class... Args>
	inline auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
	  -> std::enable_if_t<is_function_v<T> && is_reference_wrapper_v<std::decay_t<RefWrap>>,
	                      decltype((ref.get().*pmf)(std::forward<Args>(args)...))> {
		return (ref.get().*pmf)(std::forward<Args>(args)...);
	}

	template <class Base, class T, class Pointer, class... Args>
	inline auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
	  -> std::enable_if_t<is_function_v<T> && !is_reference_wrapper_v<std::decay_t<Pointer>> &&
	                        !is_base_of_v<Base, std::decay_t<Pointer>>,
	                      decltype(((*std::forward<Pointer>(ptr)).*
	                                pmf)(std::forward<Args>(args)...))> {
		return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
	}

	template <class Base, class T, class Derived>
	inline auto INVOKE(T Base::*pmd, Derived&& ref)
	  -> std::enable_if_t<!is_function_v<T> && is_base_of_v<Base, std::decay_t<Derived>>,
	                      decltype(std::forward<Derived>(ref).*pmd)> {
		return std::forward<Derived>(ref).*pmd;
	}

	template <class Base, class T, class RefWrap>
	inline auto INVOKE(T Base::*pmd, RefWrap&& ref)
	  -> std::enable_if_t<!is_function_v<T> && is_reference_wrapper_v<std::decay_t<RefWrap>>,
	                      decltype(ref.get().*pmd)> {
		return ref.get().*pmd;
	}

	template <class Base, class T, class Pointer>
	inline auto INVOKE(T Base::*pmd, Pointer&& ptr)
	  -> std::enable_if_t<!is_function_v<T> && !is_reference_wrapper_v<std::decay_t<Pointer>> &&
	                        !is_base_of_v<Base, std::decay_t<Pointer>>,
	                      decltype((*std::forward<Pointer>(ptr)).*pmd)> {
		return (*std::forward<Pointer>(ptr)).*pmd;
	}

	template <class F, class... Args>
	inline auto INVOKE(F&& f, Args&&... args)
	  -> std::enable_if_t<!is_member_pointer_v<std::decay_t<F>>,
	                      decltype(std::forward<F>(f)(std::forward<Args>(args)...))> {
		return std::forward<F>(f)(std::forward<Args>(args)...);
	}
} // namespace detail

template <class F, class... ArgTypes>
std::result_of_t<F && (ArgTypes && ...)> invoke(F&& f, ArgTypes&&... args) {
	return detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...);
}
