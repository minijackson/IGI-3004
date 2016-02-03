#pragma once

#include "utility.hpp"

/*! \brief Represents an output file.
 *
 * This class implements the RAII idiom.
 */
template <typename flag = unbuffered_flag>
class OFile : public DummyFile<flag> {
	static_assert(std::is_same<flag, buffered_flag>::value ||
	                std::is_same<flag, unbuffered_flag>::value,
	              "An IFile must either be buffered or unbuffered.");

public:
	/*! \brief Construct a dummy file writer.
	 *
	 * This object will not be associated with a file.
	 */
	OFile() noexcept;

	/*! \brief OFile constructor with file descriptor.
	 *
	 * This constructor will not open the file nor will it close it on
	 * destruction.
	 */
	OFile(int const fd, size_t const bufSize) noexcept;

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

	// Non-copyable object
	OFile(const OFile&) = delete;
	OFile& operator=(const OFile&) = delete;

	/*! \brief Move-construct the file writer.
	 *
	 * The previous writer will be transformed to a dummy OFile
	 *
	 * \param other the OFile to be moved.
	 */
	OFile(OFile&& other) noexcept;

	/*! \brief Move-assign the file writer.
	 *
	 * The previous writer will be transformed to a dummy OFile
	 *
	 * \param other the OFile to be moved.
	 * \return the moved OFile
	 */
	OFile& operator=(OFile&& other) noexcept;

	/*! \brief Write a line to the file.
	 *
	 * \param line the line to write.
	 *
	 * \return the current object (for chaining operator<<).
	 */
	OFile& operator<<(char const* line);

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

	/*! \brief Check if the object opened file.
	 *
	 * \return `true` if the object opened the file.
	 */
	bool hasOpenedTheFile() const;

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
	int openFile(char const* filename) noexcept(false);

	/*! \brief Write one line to the file.
	 *
	 * \param content the line to write.
	 */
	void writeLine(char const* content) noexcept(false);

	/*! \brief True if this object is a dummy writer.
	 */
	bool dummy = false;

	/*! \brief The file descriptor of the current file.
	 */
	int fd;

	/*! \brief The size of the buffer for reading.
	 *
	 * If a line to write in the file is longer than the buffer size, the line
	 * will be truncated.
	 */
	size_t bufSize;

	/*! \brief `true` if the current object is the one that opened the file.
	 */
	bool openedTheFile = false;
};

#include "ofile.tcc"
