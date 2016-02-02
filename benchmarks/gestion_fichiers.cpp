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

		stdFile = std::ifstream("read.dat");
		myFile  = IFile("read.dat", 2048);
	}

	void tearDown() override {
		stdFile.close();
		myFile.close();
	}

	std::ifstream stdFile;
	IFile myFile;
};

BASELINE_F(GestionFichiersReading, Baseline, ReadingFixture, 30, 1'000) {
	char line[2048];
	celero::DoNotOptimizeAway(stdFile >> line);
}

BENCHMARK_F(GestionFichiersReading, IFile, ReadingFixture, 30, 1'000) {
	char line[2048];
	celero::DoNotOptimizeAway(myFile >> line);
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

		stdFile = std::ofstream("write.dat");
		myFile  = OFile("write.dat", 2048);
	}

	void tearDown() override {
		stdFile.close();
		myFile.close();
		data.clear();
	}

	std::vector<std::string> data;

	std::ofstream stdFile;
	OFile myFile;
};

BASELINE_F(GestionFichiersWriting, Baseline, WritingFixture, 30, 1'000) {
	for(const auto& line : data) {
		celero::DoNotOptimizeAway(stdFile << line.c_str());
	}
}

BENCHMARK_F(GestionFichiersWriting, OFile, WritingFixture, 30, 1'000) {
	for(const auto& line : data) {
		celero::DoNotOptimizeAway(myFile << line.c_str());
	}
}
