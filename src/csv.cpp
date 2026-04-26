#include "csv.h"
#include <fstream>
#include <initializer_list>
#include <ostream>
#include <vector>

// std::ostream &operator<<(std::ostream &o, CSV &csv) {
//   for (auto it = csv.begin_lines(); it != csv.end_lines() - 1; ++it) {
//     auto cur_line = *it;
//     for (size_t i{}; i < csv.fields() - 1; ++i) {
//       o << cur_line[i] << ',';
//     }
//     o << cur_line[csv.fields() - 1];
//   }
//   return o;
// };

CSV::CSV(std::initializer_list<std::string> header) noexcept
    : csv_header_{header}, csv_data_{} {};

void CSV::emit_CSV(std::string filename) const {
  std::ofstream out{filename + ".csv"};
  out << *this;
};

std::ostream &operator<<(std::ostream &o, const CSV &csv) {
  auto header = csv.header();
  o << header[0];
  for (size_t i{1}; i < header.size(); ++i) {
    o << ',' << header[i];
  }

  auto data = csv.data();
  for (auto &l : data) {
    o << '\n' << l[0];
    for (size_t i{1}; i < l.size(); ++i) {
      o << ',' << l[i];
    }
  }
  return o;
}
