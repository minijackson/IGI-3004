//#include "memory.hpp"

#include <iostream>
#include <random>

constexpr size_t const n = 2048;
constexpr size_t const m = 2048;

long long unsigned int somme1(int** tableau, size_t N, size_t M) {
	long long unsigned int somme = 0;

	for(size_t i = 0; i < N; ++i) {
		for(size_t j = 0; j < M; ++j) {
			somme += tableau[i][j];
		}
	}

	return somme;
}

int main() {
	//UniquePtr<int*, MultiDimDeleter> ptr = makeMultiDimUniquePtr<int**>(n, m);
	//int** tableau = ptr.get();
	int** tableau = static_cast<int**>(malloc(n*m*sizeof(int*)));

	std::random_device rand;
	std::mt19937 gen(rand());

	for(size_t i = 0; i < n; ++i) {
		tableau[i] = static_cast<int*>(malloc(m*sizeof(int)));
		for(size_t j = 0; j < m; ++j) {
			tableau[i][j] = gen();
		}
	}

	std::cout << "Somme: " << somme1(tableau, n, m) << std::endl;

	for(size_t i = 0; i < n; ++i) {
		free(tableau[i]);
	}

	free(tableau);
}