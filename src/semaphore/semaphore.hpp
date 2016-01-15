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
	/*! \brief Semaphore default constructor.
	 *
	 * This will initialize the semaphore with 1 as initial value.
	 *
	 * \throws std::system_error when the semaphore cannot be created.
	 */
	Semaphore() throw(std::system_error);

	/*! \brief Semaphore constructor.
	 *
	 * \param value the initial value of the semaphore.
	 *
	 * \throws std::system_error when the semaphore cannot be created.
	 */
	explicit Semaphore(unsigned int value) throw(std::system_error);

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
	 void wait() throw(std::system_error);

	 /*! \brief Increase the availability of the semaphore
	  *
	  * \throws std::system_error when the semaphore value cannot be increased.
	  */
	 void post() throw(std::system_error);

	 /*! \brief Get the current value of the semaphore.
	  *
	  * \return The current value of the semaphore.
	  *
	  * \throws std::system_error when the value cannot be recovered.
	  */
	 int getValue() throw(std::system_error);

protected:
	sem_t semaphore;
};
