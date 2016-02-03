#pragma once

#include "utility.hpp"

/*! \brief Represents an input file.
 *
 * This class implements the RAII idiom.
 */
template <typename flag = unbuffered_flag>
class IFile : public DummyFile<flag> {
	static_assert(std::is_same<flag, buffered_flag>::value ||
	                std::is_same<flag, unbuffered_flag>::value,
	              "An IFile must either be buffered or unbuffered.");

public:
	/*! \brief Construct a dummy file reader.
	 *
	 * This object will not be associated with a file.
	 */
	IFile() noexcept;

	/*! \brief IFile constructor with file descriptor.
	 *
	 * This constructor will not open the file nor will it close it on
	 * destruction.
	 */
	IFile(int const fd, size_t const bufSize) noexcept;

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

	// Non-copyable object
	IFile(const IFile&) = delete;
	IFile& operator=(const IFile&) = delete;

	/*! \brief Move-construct the file reader.
	 *
	 * The previous reader will be transformed to a dummy IFile
	 *
	 * \param other the IFile to be moved.
	 */
	IFile(IFile&& other) noexcept;

	/*! \brief Move-assign the file reader.
	 *
	 * The previous reader will be transformed to a dummy IFile
	 *
	 * \param other the IFile to be moved.
	 * \return the moved IFile
	 */
	IFile& operator=(IFile&& other) noexcept;

	/*! \brief Read a line from the file.
	 *
	 * \param line the string to write the line to.
	 *
	 * \return the current object (for chaining operator>>).
	 */
	IFile& operator>>(char* line);

	/*! \brief close the file associated with this object.
	 */
	void close();

	/*! \brief Get the file's file descriptor.
	 *
	 * \return the file's file descriptor.
	 */
	int getFd() const;

	/*! \brief Get the size of the buffer.
	 *
	 * \return the size of the buffer.
	 */
	size_t getBufSize() const;

	/*! \brief Check if the end-of-file has been reached.
	 *
	 * \return `true` if the end-of-file has been reached.
	 */
	bool hasEnded() const;

	/*! \brief Check if the object opened file.
	 *
	 * \return `true` if the object opened the file.
	 */
	bool hasOpenedTheFile() const;

protected:
	/*! \brief Open the file in read-only mode.
	 *
	 * Will set the `openedTheFile` attribute to `true`.
	 *
	 * \param filename The name of the file to open.
	 *
	 * \return the opened file's file descriptor.
	 */
	int openFile(char const* filename) noexcept(false);

	/*! \brief Read one line of the file.
	 *
	 * \param buf the buffer to write the line to.
	 */
	void getLine(char* buf) noexcept(false);

	/*! \brief True if this object is a dummy reader.
	 */
	bool dummy = false;

	/*! \brief The file descriptor of the current file.
	 */
	int fd;

	/*! \brief The size of the buffer for reading.
	 *
	 * If a line in the file is longer than the buffer size, the whole line
	 * will not be returned.
	 */
	size_t bufSize;

	/*! \brief `true` if the end-of-file has been reached, `false` otherwise.
	 */
	bool ended = false;

	/*! \brief `true` if the current object is the one that opened the file.
	 */
	bool openedTheFile = false;
};

#include "ifile.tcc"
