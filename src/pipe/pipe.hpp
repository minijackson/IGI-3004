#pragma once

#include "gestion-fichiers.hpp"

/*! \brief Helper class for the Pipe class.
 *
 * Used to create the pipe and store the pipe's file descriptors before initializing the parents of
 * the Pipe class (IFile and OFile).
 *
 * \sa Pipe
 */
class DummyPipe {
public:
	/*! \brief Get the file descriptor of the pipe's reading end.
	 *
	 * \return the file descriptor of the pipe's reading end.
	 */
	int getReadingFd() const;

	/*! \brief Get the file descriptor of the pipe's writing end.
	 *
	 * \return the file descriptor of the pipe's writing end.
	 */
	int getWritingFd() const;

protected:
	/*! \brief DummyPipe constructor.
	 *
	 * Will create and open a normal anonymous pipe.
	 *
	 * \param nonBlocking `true` if the pipe must be non-blocking.
	 */
	explicit DummyPipe(bool nonBlocking) noexcept(false);

	/*! \brief The file descriptors of the opened pipe.
	 */
	int pipeFd[2];

	~DummyPipe() = default;
};

/*! \brief Represents a pipe
 *
 * See the pipe(2) man page for more informations.
 * This class implements the RAII idiom.
 */
class Pipe : public DummyPipe, public IFile<unbuffered_flag>, public OFile<unbuffered_flag> {
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

	/*! \brief Close the pipe.
	 *
	 * Will close both ends of the pipe. Called by the user and by the destructor.
	 */
	void close();

	/*! \brief Check if the pipe is non-blocking.
	 *
	 * \return `true` if the pipe is non-blocking.
	 */
	bool isNonBlocking() const;

	/*! \brief Check if reading end of the pipe is closed.
	 *
	 * \return `true` if the reading end of the pipe is closed.
	 */
	bool isReadClosed() const;

	/*! \brief Check if writing end of the pipe is closed.
	 *
	 * \return `true` if the writing end of the pipe is closed.
	 */
	bool isWriteClosed() const;

protected:
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
