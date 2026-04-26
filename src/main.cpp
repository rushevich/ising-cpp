// main.cpo

#include "lattice.h"
#include "simulation.h"
#include <iostream>

int main() {
  IsingLattice<16> lattice_16(false);
  std::cout << "Initial 16x16 lattice state:\n";
  lattice_16.draw();
  std::cout << "Initial Energy: " << lattice_16.energy() << std::endl;
  Observables results_16 = simulate(lattice_16, 1.9);
  std::cout << "State after simulation with T=1.9:\n";
  lattice_16.draw();
  std::cout << "Simulation results:\n";
  results_16.print();

  return 0;
}
