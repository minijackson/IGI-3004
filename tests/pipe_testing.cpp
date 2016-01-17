#include "pipe.hpp"

#include <sstream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>

std::string referenceText =
  "But the greatest Electrical Pioneer of them all was Thomas Edison, who was a\n"
  "brilliant inventor despite the fact that he had little formal education and\n"
  "lived in New Jersey.  Edison's first major invention in 1877, was the\n"
  "phonograph, which could soon be found in thousands of American homes, where\n"
  "it basically sat until 1923, when the record was invented.  But Edison's\n"
  "greatest achievement came in 1879, when he invented the electric company.\n"
  "Edison's design was a brilliant adaptation of the simple electrical circuit:\n"
  "the electric company sends electricity through a wire to a customer, then\n"
  "immediately gets the electricity back through another wire, then (this is\n"
  "the brilliant part) sends it right back to the customer again.\n"
  "\n"
  "This means that an electric company can sell a customer the same batch of\n"
  "electricity thousands of times a day and never get caught, since very few\n"
  "customers take the time to examine their electricity closely. In fact the\n"
  "last year any new electricity was generated in the United States was 1937;\n"
  "the electric companies have been merely re-selling it ever since, which is\n"
  "why they have so much free time to apply for rate increases.\n"
  "                -- Dave Barry, \"What is Electricity?\"\n";

BOOST_AUTO_TEST_CASE(blocking_pipe_creation) {
	Pipe pipe;

	BOOST_CHECK_NE(pipe.getReadingFd(), -1);
	BOOST_CHECK_NE(pipe.getWritingFd(), -1);
	BOOST_CHECK(!pipe.isNonBlocking());
	BOOST_CHECK(!pipe.isReadClosed());
	BOOST_CHECK(!pipe.isWriteClosed());
}

BOOST_AUTO_TEST_CASE(blocking_pipe_closing) {
	Pipe pipe, pipe2;

	BOOST_CHECK_NE(pipe.getReadingFd(), pipe2.getReadingFd());
	BOOST_CHECK_NE(pipe.getWritingFd(), pipe2.getWritingFd());

	pipe.readOnly();
	BOOST_CHECK(!pipe.isReadClosed());
	BOOST_CHECK(pipe.isWriteClosed());

	pipe2.writeOnly();
	BOOST_CHECK(pipe2.isReadClosed());
	BOOST_CHECK(!pipe2.isWriteClosed());

	pipe.writeOnly();
	BOOST_CHECK(pipe.isReadClosed());
	BOOST_CHECK(pipe.isWriteClosed());
}

BOOST_AUTO_TEST_CASE(blocking_pipe_reading_writing) {
	Pipe pipe;

	// Writing
	{
		std::istringstream stream(referenceText);

		std::string temp;

		size_t count(0);

		while(std::getline(stream, temp)) {
			pipe << temp.c_str() << "\n";
			++count;
		}

		BOOST_CHECK_EQUAL(count, 18);
	}

	// Reading
	{
		pipe.readOnly();

		std::ostringstream stream;

		auto temp = std::make_unique<char[]>(4096);

		size_t count(0);

		while(!pipe.hasEnded()) {
			pipe >> temp.get();
			stream << temp.get();
			++count;
		}

		BOOST_CHECK_EQUAL(count, 19);
		BOOST_CHECK_EQUAL(stream.str(), referenceText);
	}
}

BOOST_AUTO_TEST_CASE(non_blocking_pipe_creation) {
	Pipe pipe(/* nonBlocking = */ true);

	BOOST_CHECK_NE(pipe.getReadingFd(), -1);
	BOOST_CHECK_NE(pipe.getWritingFd(), -1);
	BOOST_CHECK(pipe.isNonBlocking());
	BOOST_CHECK(!pipe.isReadClosed());
	BOOST_CHECK(!pipe.isWriteClosed());
}
