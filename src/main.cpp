#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace {
template <unsigned N>
using Square = std::array<unsigned, N * N>;
constexpr unsigned N = 3;

bool is_magic(const Square<N>& square) {
    constexpr unsigned invariant = (N * (N * N + 1)) / 2;
    std::array<unsigned, N> line_sums = {0};
    std::array<unsigned, N> col_sums = {0};
    std::array<unsigned, 2> diag_sums = {0};

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            line_sums[i] += square[N * i + j];
            col_sums[j] += square[N * i + j];
            if (i == j) {
                diag_sums[0] += square[N * i + j];
            }
            if (N - 1 - i == j) {
                diag_sums[1] += square[N * i + j];
            }
        }
    }

    auto is_invariant = [](auto a) { return a == invariant; };
    return std::all_of(col_sums.begin(), col_sums.end(), is_invariant) &&
           std::all_of(line_sums.begin(), line_sums.end(), is_invariant) &&
           std::all_of(diag_sums.begin(), diag_sums.end(), is_invariant);
}

Square<4> nondiagonal_permutation(const Square<4>& square) {
    Square<4> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (i != j && N - 1 - i != j) {
                result[N * i + j] = N * N + 1 - square[N * i + j];
            } else {
                result[N * i + j] = square[N * i + j];
            }
        }
    }
    return result;
}

void print_square(const Square<N>& square) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::cout << square[N * i + j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}
}

int main(void) {
    Square<N> values;
    for (unsigned i = 1; i <= N * N; ++i) {
        values[i - 1] = i;
    }
    do {
        if (is_magic(values)) {
            print_square(values);
        }
    } while (std::next_permutation(values.begin(), values.end()));
    return 0;
}
