#include <tokenizer/tokenizer.hxx>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
	std::istream* in = nullptr;

	for(int i=1; i < argc; i++) {
		std::string arg = argv[i];
		if(arg == "-F") 
			if(++i < argc) {
				arg = argv[i];
				if(arg != "-") {
					std::ifstream *fin = new std::ifstream(arg);
					if(!fin->is_open()) {
						std::cerr << "Can't open the file: " << arg << std::endl;
						delete fin;
						return 1;
					}
				}
			} else {
				std::cerr << "File path not given after '-F' option" << std::endl;
				return 1;
			}
	}

	if(in == nullptr)
		in = &std::cin;

	Tokenizer::use(*in);
	Tokenizer::use(std::cout);

	Tokenizer::proc();

	if(in != &std::cin)
		delete in;
};
