#include "memory.hpp"

#include <iostream>
#include <random>

constexpr size_t const n = 2048;
constexpr size_t const m = 2048;

long long unsigned int somme1(unsigned int** tableau, size_t N, size_t M) {
	long long unsigned int somme = 0;

	for(size_t i = 0; i < N; ++i) {
		for(size_t j = 0; j < M; ++j) {
			somme += tableau[i][j];
		}
	}

	return somme;
}

int main() {
	UniquePtr<unsigned int*, MultiDimDeleter> ptr = makeMultiDimUniquePtr<unsigned int**>(n, m);
	unsigned int** tableau = ptr.get();

	std::random_device rand;
	std::mt19937 gen(rand());

	for(size_t i = 0; i < n; ++i) {
		for(size_t j = 0; j < m; ++j) {
			tableau[i][j] = gen();
		}
	}

	std::cout << "Somme: " << somme1(tableau, n, m) << std::endl;
}
