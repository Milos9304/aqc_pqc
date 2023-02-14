#include <vector>
#include <tuple>
#include <string>
#include <map>
#include "read_dataset.h"
#include "FastVQA/pauliHamiltonian.h"

std::vector<dataset_instance> read_nb_part_dataset(std::string name);
std::map<std::string, std::vector<long long int>> read_nb_part_solutions_txt(std::ifstream *f);
