#include "popl.hpp"
#include "io/logger.h"
#include "FastVQA/fastVQA.h"

#include "read_dataset.h"

using namespace popl;

int main(int ac, char** av){

	int seed = 0;
	int loglevel = 0;

	FastVQA::AcceleratorOptions acceleratorOptions;
	acceleratorOptions.accelerator_type = "quest";

	OptionParser op("Allowed options");
	auto help_option     = op.add<Switch>("h", "help", "produce help message");
	auto log_level       = op.add<Value<int>>("", "loglevel", "0 - debug, 1 - info, 2 - warning, 3 - error", 0);
	auto seed_option 	 = op.add<Value<int>>("", "seed", "seed for the experiments", seed);

	op.parse(ac, av);

	if (help_option->is_set()){
		std::cout << op << "\n";
		return 0;
	}

	if(log_level->value() < 0 || log_level->value() > 3){
		throw_runtime_error("Invalid loglevel value.");
	}
	loglevel = log_level->value();

	seed = seed_option->value();
	logi("Using seed " + std::to_string(seed), loglevel);

	std::vector<dataset_instance> dataset = read_dataset("small/andromeda");
	for(auto &instance : dataset){
		std::cout << std::get<0>(instance) << std::endl;
		std::cout << std::get<1>(instance).getHamiltonianString(1) << std::endl;
	}

	return 0;
}
