#include <vector>
#include <tuple>
#include <string>

#include "FastVQA/hamiltonian.h"

typedef std::pair<std::string, FastVQA::Hamiltonian> dataset_instance;

std::vector<dataset_instance> read_dataset(std::string name);
