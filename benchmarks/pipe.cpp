#include <celero/Celero.h>

#include "pipe.hpp"

#include <condition_variable>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

CELERO_MAIN

class ThreadCommunicationFixture : public celero::TestFixture {
public:
	ThreadCommunicationFixture() {}

	std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
		std::vector<std::pair<int64_t, uint64_t>> bufferSizes;

		bufferSizes.push_back(std::pair<int64_t, uint64_t>(32, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(45, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(64, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(90, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(128, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(181, 0));
		bufferSizes.push_back(std::pair<int64_t, uint64_t>(256, 0));

		return bufferSizes;
	}

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

	}

	std::vector<std::string> data;
};

BASELINE_F(ThreadCommunication, Perfect, ThreadCommunicationFixture, 30, 1'000) {
	std::thread myThread([this]() {
		for(const auto& line : data) {
			celero::DoNotOptimizeAway(line);
		}
	});

	myThread.join();
}

BENCHMARK_F(ThreadCommunication, Pipe, ThreadCommunicationFixture, 30, 1'000) {
	Pipe pipe;

	std::thread myThread([&pipe]() {
		char line[4096];
		pipe >> line;
		celero::DoNotOptimizeAway(std::string(line));
	});

	for(auto& line : data) {
		pipe << std::move(line.c_str());
	}

	myThread.join();
}

BENCHMARK_F(ThreadCommunication, Mutex, ThreadCommunicationFixture, 30, 1'000) {
	std::string line;
	std::mutex lineMutex;
	std::condition_variable cv;
	bool ready = false;

	std::thread myThread([&line, &lineMutex, &cv, &ready]() {
		while(line != "") {
			std::unique_lock<std::mutex> lk(lineMutex);
			cv.wait(lk, [&ready] { return ready; });

			celero::DoNotOptimizeAway(line);
		}
	});

	for(auto& lineToTransmit : data) {
		{
			std::lock_guard<std::mutex> lk(lineMutex);
			ready = true;
			line = lineToTransmit;
		}
		cv.notify_one();
	}

	line = "";
	cv.notify_one();

	myThread.join();
}
