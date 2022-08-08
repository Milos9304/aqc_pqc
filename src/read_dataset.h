#include <vector>
#include <tuple>
#include <string>
#include <map>


#include "FastVQA/pauliHamiltonian.h"

typedef std::pair<std::string, FastVQA::PauliHamiltonian> pre_instance;
typedef std::tuple<std::string, FastVQA::PauliHamiltonian, int> dataset_instance;

std::vector<dataset_instance> read_dataset(std::string name);
std::map<std::string, long long int> read_solutions_txt(std::ifstream *f);
