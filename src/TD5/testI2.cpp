#include <iostream>
#include <random>

constexpr int const n = 2048;
constexpr int const m = 2048;

long long unsigned int somme2(int** tableau, size_t N, size_t M) {
	long long unsigned int somme = 0;

	for(size_t j = 0; j < M; ++j) {
		for(size_t i = 0; i < N; ++i) {
			somme += tableau[i][j];
		}
	}

	return somme;
}

int main() {
	int** tableau = static_cast<int**>(malloc(n*m*sizeof(int*)));

	std::random_device rand;
	std::mt19937 gen(rand());

	for(int i = 0; i < n; ++i) {
		tableau[i] = static_cast<int*>(malloc(m*sizeof(int)));
		for(int j = 0; j < m; ++j) {
			tableau[i][j] = gen();
		}
	}

	std::cout << "Somme: " << somme2(tableau, n, m) << std::endl;

	for(int i = 0; i < n; ++i) {
		free(tableau[i]);
	}

	free(tableau);
}
