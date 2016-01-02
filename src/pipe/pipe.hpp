#pragma once

/*! \brief Represents a pipe
 *
 * See the pipe(2) man page for more informations.
 * This class implements the RAII idiom.
 */
class Pipe {
public:
	/*! \brief Pipe default constructor.
	 *
	 * Will create and open a normal pipe.
	 *
	 * \param nonBlocking `true` if the pipe must be non-blocking.
	 */
	explicit Pipe(bool nonBlocking = false);

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

	/*! \brief Read one line of the pipe.
	 *
	 * This method will wait for a end-of-line or end-of-transmission caracter.
	 * If none is present, it will loop infinitely.
	 *
	 * \param output the string to write the message to.
	 *
	 * \return the current object (for chaining operator>>).
	 */
	Pipe& operator>>(char output[]);

	/*! \brief Make the pipe read only.
	 *
	 * Will close the write end of the pipe.
	 */
	void readOnly();

	/*! \brief Make the pipe write only.
	 *
	 * Will close the read end of the pipe.
	 */
	void writeOnly();

protected:
	/*! \brief The file descriptors of the opened pipe.
	 */
	int pipeFd[2];

	/*! \brief `true` if the pipe is opened in non-blocking mode.
	 */
	bool nonBlocking;

	/*! \brief `true` if the read end of the pipe is closed
	 */
	bool readClosed = false;

	/*! \brief `true` if the write end of the pipe is closed
	 */
	bool writeClosed = false;
};
