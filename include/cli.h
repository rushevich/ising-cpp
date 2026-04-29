// cli.h
#ifndef ISINGCPP_CLI_H
#define ISINGCPP_CLI_H
#include <string>
#include <vector>
namespace program_options {
// CLI options, * = required
struct Options {
  size_t lattice_size = 0;  // -L *
  double temp_min = 0.0;    // -T, first val *
  double temp_max = 0.0;    // -T second val *
  bool random_init = false; // -r
  double step_size = 0.1;   // -s
  size_t prod_steps = 5000;
  size_t therm_steps = 2000;
  std::string output_file; // -o *
};

auto parse_int(std::string_view s, std::string_view flag) -> int;
auto parse_double(std::string_view s, std::string_view flag) -> double;
auto parse_size(std::string_view s, std::string_view flag) -> size_t;
auto parse_options(int argc, char *argv[]) -> Options;
auto construct_T_vec(double min, double max, double step)
    -> std::vector<double>;
void run(const Options &opts);
}; // namespace program_options
#endif