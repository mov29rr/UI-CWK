#include "migrate.hpp"

// arg csv_file database_file
int main(int argc, char *argv[]) {
  assert(argc >= 3 && "Invalid number of arguments");

  return migrate(argv[1], argv[2]);
}
