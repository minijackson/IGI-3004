#pragma once

#include <cstddef>

/*! \brief Represents an input file.
 *
 * This class implements the RAII idiom.
 */
class IFile {
public:
	/*! \brief IFile constructor with file descriptor.
	 *
	 * This constructor will not open the file nor will it close it on
	 * destruction.
	 */
	IFile(int const fd, size_t const bufSize);

	/*! \brief IFile constructor with file name.
	 *
	 * This constructor will open the file.
	 */
	IFile(char const* filename, size_t const bufSize);

	/*! \brief IFile destructor.
	 *
	 * Will close the file if the object was constructed with the file name.
	 */
	~IFile();

	/*! \brief Read a line from the file.
	 *
	 * \param line the string to write the line to.
	 *
	 * \return the current object (for chaining operator>>).
	 */
	IFile& operator>>(char* line);

	/*! \brief Check if the end-of-file has been reached.
	 *
	 * \return `true` if the end-of-file has been reached.
	 */
	bool hasEnded();
protected:
	/*! \brief Open the file in read-only mode.
	 *
	 * Will set the `openedTheFile` attribute to `true`.
	 *
	 * \param filename The name of the file to open.
	 *
	 * \return the opened file's file descriptor.
	 */
	int openFile(char const* filename);

	/*! \brief Read one line of the file.
	 *
	 * \param buf the buffer to write the line to.
	 */
	void getLine(char* buf);

	/*! \brief The file descriptor of the current file.
	 */
	int const fd;

	/*! \brief The size of the buffer for reading.
	 *
	 * If a line in the file is longer than the buffer size, the whole line
	 * will not be returned.
	 */
	size_t const bufSize;

	/*! \brief `true` if the end-of-file has been reached, `false` otherwise.
	 */
	bool ended = false;

	/*! \brief `true` if the current object is the one that opened the file.
	 */
	bool openedTheFile = false;
};

/*! \brief Represents an output file.
 *
 * This class implements the RAII idiom.
 */
class OFile {
public:
	/*! \brief OFile constructor with file descriptor.
	 *
	 * This constructor will not open the file nor will it close it on
	 * destruction.
	 */
	OFile(int const fd, size_t const bufSize);

	/*! \brief OFile constructor with file name.
	 *
	 * This constructor will open the file.
	 */
	OFile(char const* filename, size_t const bufSize);

	/*! \brief OFile destructor.
	 *
	 * Will close the file if the object was constructed with the file name.
	 */
	~OFile();

	/*! \brief Write a line to the file.
	 *
	 * \param line the line to write.
	 *
	 * \return the current object (for chaining operator<<).
	 */
	OFile& operator<<(char const* line);
protected:

	/*! \brief Open the file in write mode.
	 *
	 * Will create the file (without the executable byte) if non-existent.
	 * If the file exists, it will empty it first.
	 * Will set the `openedTheFile` attribute to `true`.
	 *
	 * \param filename The name of the file to open.
	 *
	 * \return the opened file's file descriptor.
	 */
	int openFile(char const* filename);

	/*! \brief Write one line to the file.
	 *
	 * \param content the line to write.
	 */
	void writeLine(char const* content);

	/*! \brief The file descriptor of the current file.
	 */
	int const fd;

	/*! \brief The size of the buffer for reading.
	 *
	 * If a line to write in the file is longer than the buffer size, the line
	 * will be truncated.
	 */
	size_t const bufSize;

	/*! \brief `true` if the current object is the one that opened the file.
	 */
	bool openedTheFile = false;
};
