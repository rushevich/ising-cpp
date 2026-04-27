// lattice.h
#ifndef ISINGCPP_LATTICE_H
#define ISINGCPP_LATTICE_H

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>

class IsingLattice {
  auto index(size_t row, size_t col) const -> size_t {
    // L * row + col
    size_t index = L_ * row + col;
    return index;
  };

  auto sum_neighbors(size_t idx) const -> int {
    // we must consider PBC here.
    size_t row = idx / L_;
    size_t col = idx % L_;
    // left, right, top, bottom
    int result =
        spins_[index(prev_[row], col)] + spins_[index(next_[row], col)] +
        spins_[index(row, prev_[col])] + spins_[index(row, next_[col])];

    return result;
  };

public:
  IsingLattice(size_t L, bool init_randomly)
      : L_{L}, N_{L * L}, random_{init_randomly}, rng_(std::random_device{}()),
        real_dist_(0, 1), int_dist_(0, N_ - 1) {
    spins_.reserve(N_);
    if (!random_) {
      for (size_t i{}; i < N_; ++i)
        spins_.push_back(1);
    } else {
      std::uniform_int_distribution<int> spin_gen(0, 1);
      for (size_t i{}; i < N_; ++i) {
        spins_.push_back((spin_gen(rng_)) ? 1 : -1);
      }
    }

    next_.reserve(L_);
    prev_.reserve(L_);
    for (size_t i{}; i < L_; ++i) {
      next_.push_back((i + 1) % L_);
      prev_.push_back((i + L_ - 1) % L_);
    }
  };

  auto compute_boltzmann(double T) const -> std::array<double, 5> {
    std::array<double, 5> boltzmann{1, 1, 1, std::exp(-4 / T),
                                    std::exp(-8 / T)};
    return boltzmann;
  };

  // sweeps and attempts to flip N spins
  void sweep(const std::array<double, 5> &boltzmann) {
    for (size_t flips{}; flips < N_; ++flips) {
      size_t selected_index = int_dist_(rng_);
      int energy_delta =
          2 * spins_[selected_index] * sum_neighbors(selected_index);
      if (energy_delta < 0) {
        spins_[selected_index] = -spins_[selected_index];
      } else {
        double u = real_dist_(rng_);
        double p = boltzmann[(energy_delta + 8) / 4];
        spins_[selected_index] =
            (u < p) ? -spins_[selected_index] : spins_[selected_index];
      }
    }
  };

  // N sweeps
  void thermalize(double T, size_t nSweeps = 2000) {
    std::array<double, 5> boltzmann = compute_boltzmann(T);
    for (; nSweeps > 0; --nSweeps) {
      sweep(boltzmann);
    }
  };
  // mean magnetization / spin of current config
  auto magnetization() const -> double {
    double mag =
        static_cast<double>(std::accumulate(spins_.begin(), spins_.end(), 0)) /
        N_;
    return mag;
  };

  // total energy / spin of current config
  auto energy() const -> double {
    int64_t sum{};
    for (size_t i{}; i < N_; ++i) {
      size_t row = i / L_;
      size_t col = i % L_;
      sum += spins_[i] * spins_[index(next_[row], col)];
      sum += spins_[i] * spins_[index(row, next_[col])];
    }

    double e = -static_cast<double>(sum) / N_;

    return e;
  };

  /* Utilities */
  auto size() const -> size_t { return N_; };
  auto sideLength() const -> size_t { return L_; };
  void reset() {
    if (!random_) {
      for (size_t i{}; i < N_; ++i)
        spins_[i] = 1;
    } else {
      std::uniform_int_distribution<int> spin_gen(0, 1);
      for (size_t i{}; i < N_; ++i) {
        spins_[i] = (spin_gen(rng_)) ? 1 : -1;
      }
    }
  };

  auto draw() const -> void {
    for (size_t row{}; row < L_; ++row) {
      for (size_t col{}; col < L_; ++col) {
        std::cout << (spins_[index(row, col)] == 1 ? "@ " : ". ");
      }
      std::cout << '\n';
    }
  }

private:
  size_t L_;
  size_t N_;
  bool random_;
  std::vector<int8_t> spins_;
  std::vector<size_t> next_;
  std::vector<size_t> prev_;
  std::mt19937 rng_;
  std::uniform_real_distribution<double> real_dist_;
  std::uniform_int_distribution<size_t> int_dist_;
};

#endif