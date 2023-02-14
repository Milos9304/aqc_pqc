#include <vector>
#include <tuple>
#include <string>
#include <map>
#include "read_dataset.h"
#include "FastVQA/pauliHamiltonian.h"

std::vector<dataset_instance> read_maxcut_dataset(std::string name);
std::map<std::string, std::vector<long long int>> read_maxcut_solutions_txt(std::ifstream *f);
