#ifndef AQCPQC_READ_DATASET_H
#define AQCPQC_READ_DATASET_H

#include <vector>
#include <tuple>

#include "FastVQA/pauliHamiltonian.h"

typedef std::pair<std::string, FastVQA::PauliHamiltonian> pre_instance;
typedef std::tuple<std::string, FastVQA::PauliHamiltonian, std::vector<long long int>> dataset_instance;

#endif
