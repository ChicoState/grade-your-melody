#include "melody.h"
#include <cassert>

int main() {
  auto v = gym::parse_notes("C4 D4 E4");
  assert(v.size() == 3);
  assert(v[0] == "C4");
  return 0;
}
