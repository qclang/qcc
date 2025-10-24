#include <tokenparser/tokenparser.hxx>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
	std::istream* in = nullptr;
	std::ostream* out = nullptr;
	std::shared_ptr<BlockStatement> stm_root = std::make_shared<BlockStatement>();

	for(int i=1; i < argc; i++) {
		std::string arg = argv[i];
		if(arg == "-o")
			if(++i < argc) {
				arg = argv[i];
				if(arg != "-") {
					std::ofstream *fout = new std::ofstream(arg);
					if(!fout->is_open()) {
						std::cerr << "Can't open the file: " << arg << std::endl;
						delete fout;
						return 1;
					}
					out = fout;
				}
			} else {
				std::cerr << "File path not given after '-o' option" << std::endl;
				return 1;
			}
		else {
                        std::ifstream *fin = new std::ifstream(arg);
                        if(!fin->is_open()) {
                                std::cerr << "Can't open the file: " << arg << std::endl;
                                delete fin;
                                return 1;
                        }
                        in = fin;
                }
	}

	if(in == nullptr)
		in = &std::cin;

	if(out == nullptr)
		out = &std::cout;

	Tokenparser::use(*in);
	Tokenparser::use(stm_root);

	Tokenparser::proc();

	if(in != &std::cin)
		delete in;
	if(out != &std::cout)
		delete out;
};
