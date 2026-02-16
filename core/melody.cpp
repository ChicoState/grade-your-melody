#include "melody.h"
#include <sstream>

namespace gym {
  std::vector<std::string> parse_notes(const std::string& s) {
    std::istringstream iss(s);
    std::vector<std::string> out;
    for (std::string tok; iss >> tok; ) out.push_back(tok);
    return out;
  }
}
