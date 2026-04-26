// simulation.h
#ifndef ISINGCPP_SIMULATION_H
#define ISINGCPP_SIMULATION_H
#include "lattice.h"
#include <cstdlib>
#include <iostream>

struct Observables {
  /*
  should contain:
    mean abs magnetization
    susceptibility
    mean energy per spin
    spec heat
  */
  double mean_abs_m;
  double susceptibility;
  double mean_energy_per_spin;
  double specific_heat;

  // s1: accumulated absolute value of magnetization
  // s2: accumulated absolute value of mag ^ 2
  // s3: accumulated mean energy / spin
  // s4: accumulated mean energy / spin ^2
  Observables(double s1, double s2, double s3, double s4, size_t N_spins,
              size_t M_runs, double T) {
    handle_calculations(s1, s2, s3, s4, N_spins, M_runs, T);
  };

  void print() const {
    std::cout << "Mean |m|:     " << mean_abs_m << '\n'
              << "Susceptibility: " << susceptibility << '\n'
              << "Mean energy:  " << mean_energy_per_spin << '\n'
              << "Specific heat:  " << specific_heat << '\n';
  }

private:
  void handle_calculations(double sum1, double sum2, double sum3, double sum4,
                           size_t N_spins, size_t M_runs, double T) {
    mean_abs_m = sum1 / M_runs;
    susceptibility = static_cast<double>(N_spins) / T *
                     (sum2 / M_runs - mean_abs_m * mean_abs_m);
    mean_energy_per_spin = sum3 / M_runs;
    specific_heat =
        static_cast<double>(N_spins) / (T * T) *
        (sum4 / M_runs - mean_energy_per_spin * mean_energy_per_spin);
  }
};

// 4 doubles - pass by copy fine
template <size_t dim>
auto simulate(IsingLattice<dim> &lattice, double T, size_t n_therm = 2000,
              size_t n_prod = 5000) -> Observables {
  double sum_mag{};
  double sum_mag_sq{};
  double sum_e{};
  double sum_e_sq{};

  // first thermalize
  auto boltzmann = lattice.compute_boltzmann(T);
  lattice.thermalize(T, n_therm);

  for (size_t run_count{}; run_count < n_prod; ++run_count) {
    lattice.sweep(boltzmann);
    double magnetization = lattice.magnetization();
    double energy = lattice.energy();
    sum_mag += std::abs(magnetization);
    sum_mag_sq += magnetization * magnetization;
    sum_e += energy;
    sum_e_sq += energy * energy;
  }

  Observables sim_results(sum_mag, sum_mag_sq, sum_e, sum_e_sq, lattice.size(),
                          n_prod, T);

  return sim_results;
};

#endif