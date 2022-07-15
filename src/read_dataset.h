#include <vector>
#include <tuple>
#include <string>

#include "FastVQA/pauliHamiltonian.h"

typedef std::pair<std::string, FastVQA::PauliHamiltonian> dataset_instance;

std::vector<dataset_instance> read_dataset(std::string name);
