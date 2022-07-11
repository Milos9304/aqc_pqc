#include "read_dataset.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "npy.hpp"
#include <dirent.h>
#include "io/logger.h"

#include "FastVQA/fastVQA.h"

std::vector<dataset_instance> read_file(std::string dir_path, std::string file_name){

	std::vector<dataset_instance> res;

	std::string fn = file_name;

	size_t pos = 0;
	std::string token;
	pos = fn.find("_");
	token = fn.substr(0, pos);
	int num_qs = std::stoi(token);
	fn.erase(0, pos + 1);
	pos = fn.find("_");
	token = fn.substr(0, pos);
	int seed = std::stoi(token);

	std::vector<unsigned long> shape;
	bool fortran_order;
	std::vector<int> data;
    npy::LoadArrayFromNumpy(dir_path+'/'+file_name, shape, fortran_order, data);

    if(shape[0] != shape[1])
    	throw_runtime_error("MaxCut adjacency matrix should be square");

    std::vector<double> coeffs;
    std::vector<int> pauliOpts;

    int rows = shape[0], cols = shape[1];

    double constant=0;

    for(int i = 0; i < rows; ++i){

    	/*coeffs.push_back(data[i * cols+i]);//wi=0
    	for(int q = 0; q < shape[0]; ++q)
    		pauliOpts.push_back(q == i ? 3 : 0); //PauliZ*/

		for(int j = i+1; j < cols; ++j){
			constant -= data[i * cols+j] / 2.;
			coeffs.push_back(data[i * cols+j] * 0.5); //wij
	    	for(int q = 0; q < shape[0]; ++q)
	    		pauliOpts.push_back((q == i || q == j) ? 3 : 0); //PauliZ*/
    	}
    	//res.push_back(std::pair<std::string, Hamiltonian>(file_name, h));
    }

    //identity
	coeffs.push_back(constant);
	for(int q = 0; q < shape[0]; ++q)
		pauliOpts.push_back(0);

    FastVQA::Hamiltonian h(shape[0], coeffs, pauliOpts);
    res.push_back(std::pair<std::string, FastVQA::Hamiltonian>(file_name, h));

	return res;
}

std::vector<dataset_instance> read_dataset(std::string name){

	std::vector<dataset_instance> res;

	std::string dir_path_s = "../experiment_dataset/"+name;
	char * dir_path = new char [dir_path_s.length()+1];
	std::strcpy (dir_path,  dir_path_s.c_str());

	auto dir = opendir(dir_path);
	while (auto f = readdir(dir)) {
		if (!f->d_name || f->d_name[0] == '.')
			continue; // Skip everything that starts with a dot
		auto new_instances = read_file(dir_path, f->d_name);
		res.insert(std::end(res), std::begin(new_instances), std::end(new_instances));
	}
	closedir(dir);

	return res;
}
