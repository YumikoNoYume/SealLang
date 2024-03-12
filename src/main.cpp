#include <fstream>

#include "Executor.h"

int main(int argc, char** argv) {
	std::ifstream fin;
	fin.open(argv[1]);
	fin.seekg(0, fin.end);
	int length = fin.tellg();
	fin.seekg(0, fin.beg);
	if (fin) {
		try {
			seal::Context cont;
			seal::Executor exe {cont};
			std::string program;
			program.resize(length);
			fin.read(&program.front(), length);
			std::size_t act_length = program.find_first_of('\0');
			program.resize(act_length);
			exe.Execute(program);
		}
		catch (seal::impl::Exception errors) {
			std::vector<std::string> em = errors.GetException();
			for (auto e : em) {
				std::cout << "Exception caught: " << e << ";\n";
			}
		}
	} 
	else {
		std::cerr << "Error! Unable to open the file";
	}
	return 0;
}
