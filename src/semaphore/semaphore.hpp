#pragma once

#include <system_error>

#include <semaphore.h>

/*! \brief Represents a semaphore.
 *
 * This class uses the POSIX semaphore library.
 * This class implements the RAII idiom.
 */
class Semaphore {
public:
	/*! \brief Semaphore constructor.
	 *
	 * \param value the initial value of the semaphore.
	 *
	 * \throws std::system_error when the semaphore cannot be created.
	 */
	explicit Semaphore(unsigned int value = 1) noexcept(false);

	Semaphore(const Semaphore&) noexcept(true) = default;
	Semaphore& operator=(const Semaphore&) noexcept(true) = default;
	Semaphore& operator=(Semaphore&&) noexcept(true) = default;

	/*! \brief Semaphore destructor
	 *
	 * Will destroy the semaphore. Any thread waiting for this semaphore while
	 * in destruction is undefined behavior.
	 */
	 ~Semaphore();

	 /*! \brief Wait for the semaphore to be available and decrease its value.
	  *
	  * \throws std::system_error when the semaphore cannot be waited on.
	  */
	 void wait() noexcept(false);

	 /*! \brief Increase the availability of the semaphore
	  *
	  * \throws std::system_error when the semaphore value cannot be increased.
	  */
	 void post() noexcept(false);

	 /*! \brief Get the current value of the semaphore.
	  *
	  * \return The current value of the semaphore.
	  *
	  * \throws std::system_error when the value cannot be recovered.
	  */
	 int getValue() noexcept(false);

	 /*! \brief Get the inner POSIX semaphore.
	  *
	  * \return the inner POSIX semaphore.
	  */
	 sem_t getPosixSemaphore() const;

protected:
	/*! \brief The inner POSIX semaphore.
	 */
	sem_t semaphore;
};
