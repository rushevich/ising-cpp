// simulation.h
#ifndef ISINGCPP_SIMULATION_H
#define ISINGCPP_SIMULATION_H
#include "lattice.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <utility>

struct Observables {
  /*
  should contain:
    mean abs magnetization
    susceptibility
    mean energy per spin
    spec heat
  */

  // s1: accumulated absolute value of magnetization
  // s2: accumulated absolute value of mag ^ 2
  // s3: accumulated mean energy / spin
  // s4: accumulated mean energy / spin ^2
  Observables(double s1, double s2, double s3, double s4, size_t N_spins,
              size_t M_runs, double T)
      : T_{T} {
    handle_calculations(s1, s2, s3, s4, N_spins, M_runs, T);
  };

  void print() const {
    std::cout << "Mean |m|:     " << mean_abs_m_ << '\n'
              << "Susceptibility: " << susceptibility_ << '\n'
              << "Mean energy:  " << mean_energy_per_spin_ << '\n'
              << "Specific heat:  " << specific_heat_ << '\n';
  }

  auto unpack_data() const -> std::vector<double> {
    return std::vector<double>{T_, mean_abs_m_, susceptibility_,
                               mean_energy_per_spin_, specific_heat_};
  }

private:
  void handle_calculations(double sum1, double sum2, double sum3, double sum4,
                           size_t N_spins, size_t M_runs, double T) {
    mean_abs_m_ = sum1 / M_runs;
    susceptibility_ = static_cast<double>(N_spins) / T *
                      (sum2 / M_runs - mean_abs_m_ * mean_abs_m_);
    mean_energy_per_spin_ = sum3 / M_runs;
    specific_heat_ =
        static_cast<double>(N_spins) / (T * T) *
        (sum4 / M_runs - mean_energy_per_spin_ * mean_energy_per_spin_);
  }
  double T_;
  double mean_abs_m_;
  double susceptibility_;
  double mean_energy_per_spin_;
  double specific_heat_;
};

namespace simulation {
// 4 doubles - pass by copy fine
auto simulate(IsingLattice &lattice, double T, size_t n_therm = 2000,
              size_t n_prod = 5000) -> Observables;
auto T_sweep_sim(IsingLattice &lattice, const std::vector<double> &temps,
                 size_t n_therm = 2000, size_t n_prod = 5000)
    -> std::pair<std::vector<Observables>, std::chrono::milliseconds>;

}; // namespace simulation
#endif