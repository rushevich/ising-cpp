// simulation.cpp
#include "simulation.h"

namespace simulation {
auto simulate(IsingLattice &lattice, double T, size_t n_therm, size_t n_prod)
    -> Observables {
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

auto T_sweep_sim(IsingLattice &lattice, const std::vector<double> &temps,
                 size_t n_therm, size_t n_prod)
    -> std::pair<std::vector<Observables>, std::chrono::milliseconds> {
  std::vector<Observables> results;
  results.reserve(temps.size());
  const auto start_time = std::chrono::system_clock::now();
  for (size_t i{}; i < temps.size(); ++i) {
    lattice.reset();
    results.push_back(simulate(lattice, temps[i]));
  }
  const auto stop_time = std::chrono::system_clock::now();

  auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(
      stop_time - start_time);
  return {results, run_time};
};
}; // namespace simulation