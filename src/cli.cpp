// cli.cpp
#include "cli.h"
#include "csv.h"
#include "lattice.h"
#include "simulation.h"
#include <charconv>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

namespace program_options {
#define ARG_CHECK(x, y) (x) >= (y).size()
auto parse_int(std::string_view s, std::string_view flag) -> int {
  int value{};
  auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
  if (ec != std::errc{} || ptr != s.data() + s.size()) {
    throw std::runtime_error(std::string(flag) + ": invalid integer '" +
                             std::string(s) + "'");
  }
  return value;
}

auto parse_double(std::string_view s, std::string_view flag) -> double {
  double value{};
  auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
  if (ec != std::errc{} || ptr != s.data() + s.size()) {
    throw std::runtime_error(std::string(flag) + ": invalid double '" +
                             std::string(s) + "'");
  }
  return value;
}

auto parse_size(std::string_view s, std::string_view flag) -> size_t {
  size_t value{};
  auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
  if (ec != std::errc{} || ptr != s.data() + s.size()) {
    throw std::runtime_error(std::string(flag) + ": invalid size'" +
                             std::string(s) + "'");
  }
  return value;
}

auto parse_options(int argc, char *argv[]) -> Options {
  if (argc > 64)
    throw std::runtime_error("Too many flags!");

  Options opts;
  const std::vector<std::string_view> args(argv + 1, argv + argc);

  bool saw_L = false;
  bool saw_T = false;
  bool saw_o = false;
  bool saw_s = false;
  bool saw_r = false;
  bool saw_p = false;
  bool saw_h = false;

  for (size_t i{}; i < args.size(); ++i) {
    const auto &arg = args[i];

    if (arg == "-L") {
      if (saw_L)
        throw;
      if (ARG_CHECK(i + 1, args))
        throw;
      opts.lattice_size = parse_size(args[++i], arg);
      if (opts.lattice_size <= 0)
        throw;
      saw_L = true;
      continue;
    }
    if (arg == "-T") {
      if (saw_T)
        throw;
      if (ARG_CHECK(i + 2, args))
        throw;
      opts.temp_min = parse_double(args[++i], arg);
      opts.temp_max = parse_double(args[++i], arg);
      if (opts.temp_min > opts.temp_max)
        throw;
      if (opts.temp_min < 0.0)
        throw;
      saw_T = true;
      continue;
    }
    if (arg == "-o") {
      if (saw_o)
        throw;
      if (ARG_CHECK(i + 1, args))
        throw;
      opts.output_file = std::string(args[++i]);
      saw_o = true;
      continue;
    }
    if (arg == "-s") {
      if (saw_s)
        throw;
      if (ARG_CHECK(i + 1, args))
        throw;
      opts.step_size = parse_double(args[++i], arg);
      if (opts.step_size <= 0.0)
        throw;
      saw_s = true;
      continue;
    }

    if (arg == "-r") {
      if (saw_r)
        throw;
      opts.random_init = true;
      saw_r = true;
      continue;
    }

    if (arg == "-p") {
      if (saw_p)
        throw;
      if (ARG_CHECK(i + 1, args))
        throw;
      opts.prod_steps = parse_size(args[++i], arg);
      if (opts.prod_steps <= 0)
        throw;
      saw_p = true;
      continue;
    }

    if (arg == "-h") {
      if (saw_h)
        throw;
      if (ARG_CHECK(i + 1, args))
        throw;
      opts.therm_steps = parse_size(args[++i], arg);
      if (opts.therm_steps <= 0)
        throw;
      saw_h = true;
      continue;
    }
    throw;
  }
  if (!saw_L)
    throw;
  if (!saw_T)
    throw;
  if (!saw_o)
    throw;

  return opts;
};

auto construct_T_vec(double min, double max, double step)
    -> std::vector<double> {
  std::vector<double> temps;
  for (; min < max; min += step) {
    temps.push_back(min);
  }
  return temps;
};

void run(const Options &opts) {
  IsingLattice lattice(opts.lattice_size, opts.random_init);
  std::vector<double> temps =
      construct_T_vec(opts.temp_min, opts.temp_max, opts.step_size);
  auto results = simulation::T_sweep_sim(lattice, temps, opts.therm_steps,
                                         opts.prod_steps);

  CSV results_csv({"Temp", "Mean Absolute Magnetization", "Susceptibility",
                   "Mean Energy / Spin", "Specific Heat"});

  for (auto &run_data : results.first) {
    results_csv.insert_line(run_data.unpack_data());
  }

  results_csv.emit_CSV(opts.output_file);
}
}; // namespace program_options