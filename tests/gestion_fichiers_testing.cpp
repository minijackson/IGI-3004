#include "gestion-fichiers.hpp"

#include <memory>
#include <fstream>
#include <sstream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>

constexpr size_t const TAILLEBUF = 2048;

std::string referenceText =
  "Are you a parent?  Do you sometimes find yourself unsure as to what to\n"
  "say in those awkward situations?  Worry no more...\n"
  "\n"
  "        Are you sure you're telling the truth?  Think hard.\n"
  "        Does it make you happy to know you're sending me to an early grave?\n"
  "        If all your friends jumped off the cliff, would you jump too?\n"
  "        Do you feel bad?  How do you think I feel?\n"
  "        Aren't you ashamed of yourself?\n"
  "        Don't you know any better?\n"
  "        How could you be so stupid?\n"
  "        If that's the worst pain you'll ever feel, you should be thankful.\n"
  "        You can't fool me.  I know what you're thinking.\n"
  "        If you can't say anything nice, say nothing at all.\n";

bool init_unit_test() {
	std::ofstream file("infile.txt");
	file << referenceText;
	return true;
}

int main(int argc, char* argv[]) {
	return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

BOOST_AUTO_TEST_CASE(opening_file_ro) {
	IFile file("infile.txt", TAILLEBUF);

	BOOST_CHECK_NE(file.getFd(), -1);
	BOOST_CHECK_EQUAL(file.getBufSize(), TAILLEBUF);
	BOOST_CHECK(!file.hasEnded());
	BOOST_CHECK(file.hasOpenedThefile());
}

BOOST_AUTO_TEST_CASE(opening_stdin_ro) {
	IFile file(0, TAILLEBUF);

	BOOST_CHECK_EQUAL(file.getFd(), 0);
	BOOST_CHECK_EQUAL(file.getBufSize(), TAILLEBUF);
	BOOST_CHECK(!file.hasEnded());
	BOOST_CHECK(!file.hasOpenedThefile());
}

BOOST_AUTO_TEST_CASE(opening_file_wo) {
	OFile file("outfile.txt", TAILLEBUF);

	BOOST_CHECK_NE(file.getFd(), -1);
	BOOST_CHECK_EQUAL(file.getBufSize(), TAILLEBUF);
	BOOST_CHECK(file.hasOpenedThefile());
}

BOOST_AUTO_TEST_CASE(opening_stdout_wo) {
	OFile file(1, TAILLEBUF);

	BOOST_CHECK_EQUAL(file.getFd(), 1);
	BOOST_CHECK_EQUAL(file.getBufSize(), TAILLEBUF);
	BOOST_CHECK(!file.hasOpenedThefile());
}

BOOST_AUTO_TEST_CASE(reading_file) {
	IFile file("infile.txt", TAILLEBUF);

	std::ostringstream stream;

	{
		auto temp = std::make_unique<char[]>(TAILLEBUF);

		while(!file.hasEnded()) {
			// stream << file;
			file >> temp.get();
			stream << temp.get();
		}
	}

	BOOST_CHECK_EQUAL(stream.str(), referenceText);
}

BOOST_AUTO_TEST_CASE(writing_file) {
	OFile file("outfile.txt", TAILLEBUF);

	std::istringstream stream(referenceText);

	std::string temp;

	while(std::getline(stream, temp)) {
		// stream >> file;
		file << temp.c_str();
	}

	BOOST_CHECK_EQUAL(stream.str(), referenceText);
}
