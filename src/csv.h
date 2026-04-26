// csv.h
#ifndef ISINGCPP_CSV_H
#define ISINGCPP_CSV_H
#include <initializer_list>
#include <ostream>
#include <string>
#include <vector>

/*
i think the best way to go about this is by not initializing the filestream
unless we want to read from or emit a CSV until we do that, we can just track
and manipulate a vector as an analog.
*/

class CSV {
public:
  CSV() = delete; // delete for now until we design more infra
  CSV(std::initializer_list<std::string> header) noexcept;
  void emit_CSV(std::string filename) const;
  friend std::ostream &operator<<(std::ostream &o, const CSV &csv);
  friend std::istream &operator>>(std::istream &i, const CSV &csv);
  auto lines() const -> size_t;
  auto fields() const -> size_t { return csv_header_.size(); };
  auto header() const -> const std::vector<std::string> & {
    return csv_header_;
  };
  auto data() const -> const std::vector<std::vector<double>> & {
    return csv_data_;
  }
  auto begin_lines() const { return csv_data_.begin(); };
  auto end_lines() const { return csv_data_.end(); };

  void insert_line(std::vector<double> data) { csv_data_.push_back(data); };

private:
  std::vector<std::string> csv_header_;
  std::vector<std::vector<double>> csv_data_;
};

#endif