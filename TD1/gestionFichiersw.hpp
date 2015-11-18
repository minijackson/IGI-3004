#include <cstddef>

class IFile {
public:
	IFile(int const fd, size_t const bufSize);
	IFile(char const* filename, size_t const bufSize);

	~IFile();

	IFile& operator>>(char* line);
	bool hasEnded();
protected:
	int openFile(char const* filename);
	void getLine(char* buf);

	int const fd;
	size_t const bufSize;

	bool ended = false;
};

class OFile {
public:
	OFile(int const fd, size_t const bufSize);
	OFile(char const* filename, size_t const bufSize);

	~OFile();

	OFile& operator<<(char const* line);
protected:
	int openFile(char const* filename);
	void writeLine(char const* content);

	int const fd;
	size_t const bufSize;
};
