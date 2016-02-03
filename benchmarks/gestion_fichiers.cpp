#include <celero/Celero.h>

#include "gestion-fichiers.hpp"

#include <fstream>
#include <random>
#include <vector>

CELERO_MAIN

class BaseFixture : public celero::TestFixture {
public:
	BaseFixture() {}

	std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
		std::vector<std::pair<int64_t, uint64_t>> bufferSizes;

		bufferSizes.push_back(std::pair<int64_t, uint64_t>(32, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(64, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(128, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(256, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(512, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(1024, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(2048, 0));
		//bufferSizes.push_back(std::pair<int64_t, uint64_t>(4096, 0));

		return bufferSizes;
	}

	double getExperimentValueResultScale() const override {
		// In Megabytes
		return 1024.0 * 1024.0;
	}

	void setUp(int64_t experimentValue) override = 0;
};

class ReadingFixture : public BaseFixture {
public:
	void setUp(int64_t experimentValue) override {
		std::random_device r;
		std::seed_seq seed{r(), r(), r(), r(), r(), r()};
		std::mt19937 gen(seed);

		std::ofstream filler("read.dat", std::ios::out | std::ios::trunc);

		for(int64_t i = 0; i < experimentValue; ++i) {
			for(int64_t j = 0; j < experimentValue; ++j) {
				char dat = gen() % 256;
				filler << dat;
			}
			filler << std::endl;
		}

		stdFile          = std::ifstream("read.dat");
		myUnbufferedFile = IFile<unbuffered_flag>("read.dat", 2048);

		my4096BufferedFile = IFile<buffered_flag>("read.dat", 4096);
		my2048BufferedFile = IFile<buffered_flag>("read.dat", 2048);
		my1024BufferedFile = IFile<buffered_flag>("read.dat", 1024);
		my512BufferedFile  = IFile<buffered_flag>("read.dat", 512);
		my256BufferedFile  = IFile<buffered_flag>("read.dat", 256);
	}

	void tearDown() override {
		stdFile.close();
		myUnbufferedFile.close();

		my4096BufferedFile.close();
		my2048BufferedFile.close();
		my1024BufferedFile.close();
		my512BufferedFile.close();
		my256BufferedFile.close();
	}

	std::ifstream stdFile;
	IFile<unbuffered_flag> myUnbufferedFile;

	IFile<buffered_flag> my4096BufferedFile;
	IFile<buffered_flag> my2048BufferedFile;
	IFile<buffered_flag> my1024BufferedFile;
	IFile<buffered_flag> my512BufferedFile;
	IFile<buffered_flag> my256BufferedFile;
};

BASELINE_F(GestionFichiersReading, StdFile, ReadingFixture, 30, 1'000) {
	char line[2048];
	stdFile.getline(line, 2047);
	celero::DoNotOptimizeAway(std::strcat(line, "\n"));
}

BENCHMARK_F(GestionFichiersReading, UnbufferedIFile, ReadingFixture, 30, 1'000) {
	char line[2048];
	celero::DoNotOptimizeAway(myUnbufferedFile >> line);
}

BENCHMARK_F(GestionFichiersReading, 4096BufferedIFile, ReadingFixture, 30, 1'000) {
	char line[2048];
	celero::DoNotOptimizeAway(my4096BufferedFile >> line);
}

BENCHMARK_F(GestionFichiersReading, 2048BufferedIFile, ReadingFixture, 30, 1'000) {
	char line[2048];
	celero::DoNotOptimizeAway(my2048BufferedFile >> line);
}

BENCHMARK_F(GestionFichiersReading, 1024BufferedIFile, ReadingFixture, 30, 1'000) {
	char line[2048];
	celero::DoNotOptimizeAway(my1024BufferedFile >> line);
}

BENCHMARK_F(GestionFichiersReading, 512BufferedIFile, ReadingFixture, 30, 1'000) {
	char line[2048];
	celero::DoNotOptimizeAway(my512BufferedFile >> line);
}

BENCHMARK_F(GestionFichiersReading, 256BufferedIFile, ReadingFixture, 30, 1'000) {
	char line[2048];
	celero::DoNotOptimizeAway(my256BufferedFile >> line);
}

class WritingFixture : public BaseFixture {
public:
	void setUp(int64_t experimentValue) override {
		std::random_device r;
		std::seed_seq seed{r(), r(), r(), r(), r(), r()};
		std::mt19937 gen(seed);

		for(int64_t i = 0; i < experimentValue; ++i) {
			std::string line;
			for(int64_t j = 0; j < experimentValue; ++j) {
				line.push_back(gen() % 256);
			}
			line.push_back('\n');
			data.push_back(line);
		}

		stdFile          = std::ofstream("write.dat");
		myUnbufferedFile = OFile<unbuffered_flag>("write.dat", 2048);
		//myBufferedFile   = OFile<buffered_flag>("write.dat", 2048);
	}

	void tearDown() override {
		stdFile.close();
		myUnbufferedFile.close();
		//myBufferedFile.close();
		data.clear();
	}

	std::vector<std::string> data;

	std::ofstream stdFile;
	OFile<unbuffered_flag> myUnbufferedFile;
	//OFile<buffered_flag> myBufferedFile;
};

BASELINE_F(GestionFichiersWriting, Baseline, WritingFixture, 30, 1'000) {
	for(const auto& line : data) {
		celero::DoNotOptimizeAway(stdFile << line.c_str());
	}
}

BENCHMARK_F(GestionFichiersWriting, UnbufferedOFile, WritingFixture, 30, 1'000) {
	for(const auto& line : data) {
		celero::DoNotOptimizeAway(myUnbufferedFile << line.c_str());
	}
}

//BENCHMARK_F(GestionFichiersWriting, bufferedOFile, WritingFixture, 30, 1'000) {
	//for(const auto& line : data) {
		//celero::DoNotOptimizeAway(myBufferedFile << line.c_str());
	//}
//}
