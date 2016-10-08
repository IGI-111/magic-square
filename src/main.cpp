#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <set>
#include <thread>
#include <vector>

namespace {
template <unsigned N>
using Square = std::array<unsigned, N * N>;
constexpr unsigned N = 4;

void print_square(const Square<N>& square) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::cout << square[N * i + j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

void print_vector(const Square<N>& square) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::cout << square[N * i + j] << ' ';
        }
    }
    std::cout << std::endl;
}

bool possibly_magic(Square<N> square, size_t accepted_index) {
    constexpr unsigned invariant = (N * (N * N + 1)) / 2;

    // a new line is acceptable
    if (accepted_index > 0 && accepted_index % N == 0) {
        const size_t current_line = (accepted_index - 1) / N;
        unsigned total = 0;
        for (size_t x = 0; x < N; ++x) {
            total += square[N * current_line + x];
        }
        if (total != invariant) {
            return false;
        }
    }
    // a new column is acceptable
    if (accepted_index > N * N - N) {
        const size_t current_column = (accepted_index - 1) - (N * N - N);
        unsigned total = 0;
        for (size_t y = 0; y < N; ++y) {
            total += square[N * y + current_column];
        }
        if (total != invariant) {
            return false;
        }
    }
    // the NE-SW diagonal is acceptable
    if (accepted_index == N * N - N + 1) {
        unsigned total = 0;
        for (size_t i = 0; i < N; ++i) {
            total += square[N * (N - i - 1) + i];
        }
        if (total != invariant) {
            return false;
        }
    }
    // the NW-SE diagonal is acceptable
    if (accepted_index == N * N) {
        unsigned total = 0;
        for (size_t i = 0; i < N; ++i) {
            total += square[N * i + i];
        }
        if (total != invariant) {
            return false;
        }
    }
    return true;
}

void magic_squares_rec(Square<N> current, size_t accepted_index, unsigned& magic) {
    if (possibly_magic(current, accepted_index)) {
        if (accepted_index >= (N * N) - 1) {
            ++accepted_index;
            // last cutoff is unnecessary because we have a multiplicity of one
            if (possibly_magic(current, accepted_index)) {
                // print_square(current);
                ++magic;
                std::cout << magic << " - ";
                print_vector(current);
                return;
            }
            return;
        }
        // call for every first value in the remaining ones
        for (size_t i = accepted_index; i < N * N; ++i) {
            std::iter_swap(current.begin() + accepted_index, current.begin() + i);
            magic_squares_rec(current, accepted_index + 1, magic);
        }
    }
}
}

int main(void) {
    Square<N> values;
    for (unsigned i = 1; i <= N * N; ++i) {
        values[i - 1] = i;
    }
    unsigned total = 0;

    const unsigned concurrency = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    for (unsigned t = 1; t < concurrency; ++t) {
        threads.emplace_back(
                [concurrency, t](Square<N> values, unsigned & total) {
                for (size_t i = t * ((N * N) / concurrency); i < (t + 1) * ((N * N) / concurrency);
                        ++i) {
                std::iter_swap(values.begin(), values.begin() + i);
                magic_squares_rec(values, 1, std::ref(total));
                }
                },
                values, std::ref(total));
    }
    for (size_t i = 0; i < ((N * N) / concurrency);
            ++i) {
        std::iter_swap(values.begin(), values.begin() + i);
        magic_squares_rec(values, 1, std::ref(total));
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Total number of magic squares: " << total << std::endl;
    return 0;
}
