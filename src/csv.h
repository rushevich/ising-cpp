// csv.h
#ifndef ISINGCPP_CSV_H
#define ISINGCPP_CSV_H
#include <fstream>
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
  CSV() noexcept;
  void emit_CSV();
  friend std::ostream &operator<<(std::ostream &o, CSV &csv);
  friend std::istream &operator>>(std::istream &i, CSV &csv);

private:
  std::fstream file;
  std::vector<std::vector<std::string>> csv;
};

#endif