#include "popl.hpp"
#include "io/logger.h"
#include "FastVQA/fastVQA.h"

#include "read_dataset.h"
#include <algorithm>

using namespace popl;

int main(int ac, char** av){

	int seed = 0;
	int loglevel = 0;

	OptionParser op("Allowed options");
	auto help_option     = op.add<Switch>("h", "help", "produce help message");
	auto log_level       = op.add<Value<int>>("l", "loglevel", "0 - debug, 1 - info, 2 - warning, 3 - error", 1);
	auto seed_option 	 = op.add<Value<int>>("", "seed", "seed for the experiments", seed);
	auto round_decimals	 = op.add<Value<int>>("r", "round", "round to n decimal places. n=-1 avoids rounding", 5);
	auto opt_strategy    = op.add<Value<int>>("o", "opt", "optimization strategy. 0=trivially, 1=rank_reduction", 0);
	auto num_steps		 = op.add<Value<int>>("s", "steps", "number of steps", 20);
	auto xtol			 = op.add<Value<double>>("", "xtol", "xtol", 10e-5);
	auto dataset_name	 = op.add<Value<std::string>>("d", "dataset", "Dataset name", "");

	op.parse(ac, av);

	if (help_option->is_set()){
		std::cout << op << "\n";
		return 0;
	}

	if(log_level->value() < 0 || log_level->value() > 3){
		throw_runtime_error("Invalid loglevel value.");
	}
	loglevel = log_level->value();

	if(opt_strategy->value() < 0 || opt_strategy->value() > 1)
		throw_runtime_error("Invalid opt_strategy value.");

	if(dataset_name->value() == ""){
		throw_runtime_error("No dataset specified");
	}

	seed = seed_option->value();
	logi("Using seed " + std::to_string(seed), loglevel);

	FastVQA::AqcPqcAcceleratorOptions acceleratorOptions;
	acceleratorOptions.log_level = log_level->value();
	acceleratorOptions.roundDecimalPlaces = round_decimals->value();
	acceleratorOptions.optStrategy = opt_strategy->value();
	acceleratorOptions.accelerator_type = "quest";
	acceleratorOptions.nbSteps = num_steps->value();
	acceleratorOptions.ansatz_name = "Ry_Cz_nn_Ry";/*"Ry_CNOT_nn_Rz_CNOT_Rz"*/;/**/;
	acceleratorOptions.xtol = xtol->value();
	acceleratorOptions.compareWithClassicalEigenSolver = true;
	acceleratorOptions.outputLogToFile = true;
	acceleratorOptions.checkHessian = true;
	acceleratorOptions.printGroundStateOverlap = true;
	acceleratorOptions.initialGroundState = FastVQA::InitialGroundState::PlusState;

	std::vector<dataset_instance> dataset = read_maxcut_dataset("small/"+dataset_name->value());
	//std::vector<dataset_instance> dataset = read_maxcut_dataset("small/backward");

	std::sort(dataset.begin(), dataset.end(), [](auto &a, auto &b){return 2*std::get<0>(a)[0]+std::get<0>(a)[2]<2*std::get<0>(b)[0]+std::get<0>(b)[2];});
	//int i = 0;
	for(auto &instance : dataset){

		//if(i++ != 0)
		//	continue;

		std::string instance_name = std::get<0>(instance);

		logi("Running " + instance_name);
		FastVQA::PauliHamiltonian h1 = std::get<1>(instance);
		//std::cerr<<h1.getMatrixRepresentation2(false)<<std::endl;throw;//.block(0,0,5,5)<<std::endl;

		std::vector<long long int> solutions = std::get<2>(instance);
		acceleratorOptions.solutions = solutions;
		//std::cout << h1.getPauliHamiltonianString(1) << std::endl;

		acceleratorOptions.logFileName = instance_name.substr(0, instance_name.size()-4)+".log";
		FastVQA::AqcPqcAccelerator accelerator(acceleratorOptions);

		FastVQA::PauliHamiltonian h0(h1.nbQubits);
		h0.initializeSumMinusSigmaXHamiltonian();

		accelerator.initialize(&h0, &h1);
		accelerator.run();

		break;

	}

	return 0;
}
