#include "popl.hpp"
#include "io/logger.h"
#include "FastVQA/fastVQA.h"
#include "read_maxcut_dataset.h"
#include "read_nb_dataset.h"
#include "read_ftim_dataset.h"
#include <algorithm>

using namespace popl;

int main(int ac, char** av){

	int seed = 0;
	int loglevel = 0;

	OptionParser op("Allowed options");
	auto help_option       = op.add<Switch>("h", "help", "produce help message");
	auto log_level         = op.add<Value<int>>("l", "loglevel", "0 - debug, 1 - info, 2 - warning, 3 - error", 1);
	auto seed_option 	   = op.add<Value<int>>("", "seed", "seed for the experiments", seed);
	auto round_decimals	   = op.add<Value<int>>("r", "round", "round to n decimal places. n=-1 avoids rounding", 5);
	auto opt_strategy      = op.add<Value<int>>("o", "opt", "optimization strategy. 0=trivially, 1=rank_reduction", 0);
	auto num_steps		   = op.add<Value<int>>("s", "steps", "number of steps", 20);
	auto xtol			   = op.add<Value<double>>("", "xtol", "xtol", 10e-5);
	auto catol			   = op.add<Value<double>>("c", "catol", "catol", 0.0002);
	auto dataset_name	   = op.add<Value<std::string>>("d", "dataset", "Dataset name", "");
	auto nb_part_dataset   = op.add<Value<std::string>>("p", "dataset2", "Dataset name for number partitioning", "");
	auto ftim_dataset      = op.add<Value<std::string>>("f", "dataset3", "Dataset name for FTIM problem", "");
	auto q_select          = op.add<Value<int>>("q", "num_qubits", "if set, only this number of qubits is being run. other experiments are skipped", -1);
	auto s_select 		   = op.add<Value<int>>("", "seedselect", "if set, only this number of seed is being run. other experiments are skipped", -1);
	auto classical_esolver = op.add<Switch>("e", "", "run classical eigensolver and compare");
	auto eps_print   	   = op.add<Switch>("", "eps", "print all epsilons + inequality constraint");
	auto eval_limit_step   = op.add<Value<int>>("i", "iters", "max iterations per step", 600);
	auto log_prefix		   = op.add<Value<std::string>>("", "logprefix", "log file prefix", "");
	auto extract_evals	   = op.add<Switch>("", "evals", "save 10 eigenvalues to evals.txt only");


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

	if(dataset_name->value() == "" &&  nb_part_dataset->value() == "" && ftim_dataset->value() == ""){
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
	acceleratorOptions.catol = catol->value();
	acceleratorOptions.compareWithClassicalEigenSolver = classical_esolver->is_set();
	acceleratorOptions.outputLogToFile = true;
	acceleratorOptions.checkHessian = true;
	acceleratorOptions.printGroundStateOverlap = true;
	acceleratorOptions.printEpsilons = eps_print->is_set();
	acceleratorOptions.eval_limit_step = eval_limit_step->value();
	acceleratorOptions.initialGroundState = FastVQA::InitialGroundState::PlusState;

	std::vector<dataset_instance> dataset;
	if(dataset_name -> is_set())
		dataset = read_maxcut_dataset("small/"+dataset_name->value());
	else if(nb_part_dataset -> is_set())
		dataset = read_nb_part_dataset("small/"+nb_part_dataset->value());
	else if(ftim_dataset -> is_set())
		dataset = read_ftim_dataset("small/"+ftim_dataset->value());
	else
		throw_runtime_error("Must specify a dataset");


	//std::vector<dataset_instance> dataset = read_maxcut_dataset("small/backward");

	std::sort(dataset.begin(), dataset.end(), [](auto &a, auto &b){return 2*std::get<0>(a)[0]+std::get<0>(a)[2]<2*std::get<0>(b)[0]+std::get<0>(b)[2];});

	for(auto &instance : dataset){

		std::string instance_name = std::get<0>(instance);
		std::string a = instance_name.substr(instance_name.find('_') + 1);

		logi("Running " + instance_name);

		FastVQA::PauliHamiltonian h1 = std::get<1>(instance);

		if(q_select->value() != -1 && q_select->value() != h1.nbQubits)
			continue;

		if(s_select->value() > -1 && s_select->value() != std::stoi(a.substr(0, a.size()-4)))
			continue;

		if(extract_evals->is_set()){
			Eigen::Matrix<qreal, Eigen::Dynamic, Eigen::Dynamic> m = h1.getMatrixRepresentation2(true);

			std::vector<double> evals;
			std::ofstream f(instance_name + "_evals.txt");

			for(long long int i = 0; i < m.rows(); i++){
				std::complex c = m(i,i);
				if(c.imag() != 0)
					loge("Imaginary part of eigenvalue is non-zero: " + std::to_string(c.imag()));
				evals.push_back(c.real());
			}
			std::sort(evals.begin(), evals.end());
			std::vector<double>::iterator it = evals.begin(), prev=evals.end()-1;

			int n = 10;

			for(int i = 0; i < n && it != evals.end(); ++it){
				if(*prev == *it)
					continue;
				i++;
				f << *it << std::endl;
				prev = it;
			}

			f.close();
			continue;

		}


		//std::cerr<<h1.getMatrixRepresentation2(false)<<std::endl;throw;//.block(0,0,5,5)<<std::endl;

		std::vector<long long int> solutions = std::get<2>(instance);
		acceleratorOptions.solutions = solutions;
		//std::cout << h1.getPauliHamiltonianString(1) << std::endl;

		acceleratorOptions.logFileName = log_prefix->value()+instance_name.substr(0, instance_name.size()-4)+".log";
		FastVQA::AqcPqcAccelerator accelerator(acceleratorOptions);

		FastVQA::PauliHamiltonian h0(h1.nbQubits);
		h0.initializeSumMinusSigmaXHamiltonian();

		accelerator.initialize(&h0, &h1);
		accelerator.run();

	}

	return 0;
}
