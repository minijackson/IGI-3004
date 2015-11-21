#pragma once

/*! \brief Represents a pipe
 *
 * See the pipe(2) man page for more informations.
 * This Object implements the RAII idiom.
 */
class Pipe {
public:
	/*! \brief Pipe default constructor.
	 *
	 * Will create and open a normal pipe.
	 */
	Pipe();

	/*! \brief Pipe destructor.
	 *
	 * Will close the pipe.
	 */
	~Pipe();

	/*! \brief Write in the pipe.
	 *
	 * \param message the message to write.
	 *
	 * \return the current object (for chaining operator<<).
	 */
	Pipe& operator<<(char const message[]);

	/*! \brief Read on line of the pipe.
	 *
	 * \param output the string to write the message to.
	 *
	 * \return the current object (for chaining operator>>).
	 */
	Pipe& operator>>(char output[]);

protected:
	/*! \brief The file descriptors of the opened pipe.
	 */
	int pipeFd[2];
};
