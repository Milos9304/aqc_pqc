#include "read_nb_dataset.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "npy.hpp"
#include <dirent.h>
#include "io/logger.h"

#include "FastVQA/fastVQA.h"

std::vector<pre_instance> read_ftim_file(std::string dir_path, std::string file_name){

	std::vector<pre_instance> res;

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
	std::vector<double> data;
    try{
    	npy::LoadArrayFromNumpy(dir_path+'/'+file_name, shape, fortran_order, data);
    }catch(...){
    	throw_runtime_error("Cannot read dataset file");
    }

    int rows = shape[0];
    std::vector<qreal> coeffs;
    std::vector<int> pauliOpts;

    for(int i = 0; i < rows-1; ++i){
		coeffs.push_back(-data[i]);
		//std::cerr<<i<<" "<< ((i+1)%(rows-1)) << std::endl;
		for(int q = 0; q < rows-1; ++q)
			pauliOpts.push_back( (q == i || q == ((i+1)%(rows-1)) ) ? 3 : 0 );
    }

    for(int q = 0; q < rows-1; ++q){
    	coeffs.push_back(data[rows-1]);
		for(int q2 = 0; q2 < rows-1; ++q2){
			pauliOpts.push_back( (q == q2 ) ? 1 : 0 );
		}
    }

	FastVQA::PauliHamiltonian h(rows-1, coeffs, pauliOpts);
    res.push_back(std::pair<std::string, FastVQA::PauliHamiltonian>(file_name, h));

	return res;
}

/*std::map<std::string, std::vector<long long int>> read_nb_part_solutions_txt(std::ifstream *f){

	std::map<std::string, std::vector<long long int>> res;
	std::string line;
	while (std::getline(*f, line)){
	    std::istringstream iss(line);
	    std::string name="";
	    std::vector<long long int> sols;
	    //if (!(iss >> name >> sol))
	    //	throw_runtime_error("Invalid solutions.txt format");
	    std::string temp;
	    std::string x;
	    while(std::getline(iss, temp, ' ')){
	    	if (std::istringstream(temp) >> x) { // or std::stoi(), std::strtol(), etc
	    		if(name == ""){
	    			name = x;
	    			continue;
				}
	    		sols.push_back(std::stoll(x));
			}
	    }
	    res[name] = sols;
	}

	return res;

}*/

std::vector<dataset_instance> read_ftim_dataset(std::string name){

	std::vector<dataset_instance> res;
	std::string dir_path_s = "../experiment_dataset/"+name;
	char * dir_path = new char [dir_path_s.length()+1];
	std::strcpy (dir_path,  dir_path_s.c_str());

	auto dir = opendir(dir_path);
	try{
	while (auto f = readdir(dir)) {
		if (!f->d_name || f->d_name[0] == '.')
			continue; // Skip everything that starts with a dot



		std::string fn = f->d_name;
		if(fn.size() > 6 && fn[fn.length()-5] == 't')
			continue;

		for(auto &instance : read_ftim_file(dir_path, f->d_name)){
			std::string name = std::get<0>(instance);
			//if(!solutions.count(name.substr(0, name.size()-4)))
			//	throw_runtime_error("Entry missing in solutions.txt");
			res.push_back(dataset_instance(name, std::get<1>(instance), {}));
		}
		//res.insert(std::end(res), std::begin(new_instances), std::end(new_instances));
	}}catch(...){
		throw_runtime_error("Error reading dataset");
	}
	closedir(dir);
	delete dir_path;
	return res;
}


