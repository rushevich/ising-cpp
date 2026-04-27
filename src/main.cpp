// main.cpo

#include "cli.h"
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char *argv[]) {
  // IsingLattice<16> lattice_16(true);
  // // std::cout << "Initial 16x16 lattice state:\n";
  // // lattice_16.draw();
  // // std::cout << "Initial Energy: " << lattice_16.energy() << std::endl;
  // // Observables results_16 = simulate(lattice_16, 1.9);
  // // std::cout << "State after simulation with T=1.9:\n";
  // // lattice_16.draw();
  // // std::cout << "Simulation results:\n";
  // // results_16.print();
  // std::vector<double> temps{1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8,
  //                           1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7,
  //                           2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5};

  // auto results = T_sweep_sim(lattice_16, temps, true);

  // std::vector<std::chrono::milliseconds> run_times;

  // for (auto &run_data : results.first) {
  //   results_csv.insert_line(run_data.unpack_data());
  // }

  // results_csv.emit_CSV("sim_results");
  // std::cout << "Time to simulate (ms): " << results.second << std::endl;
  try {
    const program_options::Options opts =
        program_options::parse_options(argc, argv);
    program_options::run(opts);
  } catch (const std::exception &e) {
    std::cerr << "ising-cpp: " << e.what() << '\n';
    std::cerr
        << "usage: ising-cpp -L <size> -T <min> <max> [-r] [-s <step>] -o "
           "<file.csv>\n";
    return EXIT_FAILURE;
  }
  return 0;
}
