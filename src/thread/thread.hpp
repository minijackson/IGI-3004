#pragma once

#include <string>
#include <system_error>
#include <utility>

#include <pthread.h>

/*! \brief Represents a thread.
 *
 * This class uses the POSIX pthread library.
 * This class uses the RAII idiom.
 */
class Thread {
public:
	/*! \brief Thread constructor.
	 *
	 * This will create but not run the thread.
	 */
	explicit Thread();

	Thread(const Thread&) = delete;
	Thread& operator=(const Thread&) = delete;

	/*! \brief Thread destructor.
	 *
	 * This will crash if an associated thread is still joinable.
	 */
	~Thread();

	/*! \brief Start the thread.
	 *
	 * \param threadMainFunction The main function of the thread.
	 * \param args the arguments to be passed to the thread's main function.
	 *
	 * \throws std::system_error when the thread cannot be created.
	 */
	template <typename Functor, typename... Arguments>
	void start(Functor&& threadMainFunction, Arguments&&... args) noexcept(false);

	/*! \brief Detach the thread.
	 *
	 * \throws std::system_error when the thread cannot be detached.
	 */
	void detach() noexcept(false);

	/*! \brief Join the thread.
	 *
	 * \throws std::system_error when the thread cannot be joined.
	 */
	void join() noexcept(false);

	pthread_t& getThread() { return thread; }

protected:
	pthread_t thread;

	bool joinable = false;

	/*! \brief Create the wrapper that will be called by pthread_create
	 *
	 * The wrapper function will forward the arguments to the user's function
	 *
	 * \param seq an index_sequence with the indices of the data tuple.
	 */
	template <typename Functor, typename... Arguments, size_t... I>
	static constexpr auto makeWrapperFunction(std::index_sequence<0, I...> seq);
};

#include "thread.tcc"
