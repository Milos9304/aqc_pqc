#include "popl.hpp"
#include "io/logger.h"
#include "FastVQA/fastVQA.h"

#include "read_dataset.h"

using namespace popl;

int main(int ac, char** av){

	int seed = 0;
	int loglevel = 0;

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

	FastVQA::AqcPqcAcceleratorOptions acceleratorOptions;
	acceleratorOptions.accelerator_type = "quest";
	acceleratorOptions.nbSteps = 50;
	acceleratorOptions.ansatz_name = "Ry_CNOT_all2all_Rz";

	std::vector<dataset_instance> dataset = read_dataset("small/andromeda");
	for(auto &instance : dataset){
		logi("Running " + std::get<0>(instance));
		//std::cout << h1.getHamiltonianString(1) << std::endl;

		FastVQA::Hamiltonian h1 = std::get<1>(instance);
		FastVQA::AqcPqcAccelerator accelerator(acceleratorOptions);

		FastVQA::Hamiltonian h0(h1.nbQubits);
		h0.initializeMinusSigmaXHamiltonian();

		accelerator.initialize(&h0, &h1);
		accelerator.run();

	}

	return 0;
}
