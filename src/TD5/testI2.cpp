#include "memory.hpp"

#include <iostream>
#include <random>

constexpr size_t const n = 2048;
constexpr size_t const m = 2048;

unsigned long long somme2(unsigned long** tableau, size_t N, size_t M) {
	unsigned long long somme = 0;

	for(size_t j = 0; j < M; ++j) {
		for(size_t i = 0; i < N; ++i) {
			somme += tableau[i][j];
		}
	}

	return somme;
}

int main() {
	UniquePtr<unsigned long*, MultiDimDeleter> ptr = makeMultiDimUniquePtr<unsigned long**>(n, m);
	unsigned long** tableau = ptr.get();

	std::random_device rand;
	std::seed_seq seed{rand(), rand(), rand(), rand(), rand(), rand()};
	std::mt19937 gen(seed);

	for(size_t i = 0; i < n; ++i) {
		for(size_t j = 0; j < m; ++j) {
			tableau[i][j] = gen();
		}
	}

	std::cout << "Somme: " << somme2(tableau, n, m) << std::endl;
}
