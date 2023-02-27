#ifndef AQCPQC_READ_FTIM_DATASET_H
#define AQCPQC_READ_FTIM_DATASET_H

#include <vector>
#include <tuple>
#include <string>
#include <map>
#include "read_dataset.h"
#include "FastVQA/pauliHamiltonian.h"

std::vector<dataset_instance> read_ftim_dataset(std::string name);

#endif
