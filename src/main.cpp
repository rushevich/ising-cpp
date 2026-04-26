// main.cpo

#include "csv.h"
#include "lattice.h"
#include "simulation.h"

int main() {
  IsingLattice<16> lattice_16(true);
  // std::cout << "Initial 16x16 lattice state:\n";
  // lattice_16.draw();
  // std::cout << "Initial Energy: " << lattice_16.energy() << std::endl;
  // Observables results_16 = simulate(lattice_16, 1.9);
  // std::cout << "State after simulation with T=1.9:\n";
  // lattice_16.draw();
  // std::cout << "Simulation results:\n";
  // results_16.print();
  std::vector<double> temps{1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8,
                            1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7,
                            2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5};

  auto results = T_sweep_sim(lattice_16, temps, true);

  CSV results_csv({"Temp", "Mean Absolute Magnetization", "Susceptibility",
                   "Mean Energy / Spin", "Specific Heat"});

  for (auto &run_data : results) {
    results_csv.insert_line(run_data.unpack_data());
  }

  results_csv.emit_CSV("sim_results");
  return 0;
}
