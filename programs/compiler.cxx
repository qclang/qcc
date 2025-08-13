#include <cstdlib>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <iostream>

inline
std::string getExecutablePath() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

inline
std::string getExecutableDir() {
    std::string path = getExecutablePath();
    return path.substr(0, path.find_last_of('/'));
}


int main(int argc, char *argv[]) {
	std::string in, out="a.out";

	for(int i=1; i < argc; i++) {
                std::string arg = argv[i];
                if(arg == "-o")
                        if(++i < argc) {
                                arg = argv[i];
                                if(arg != "-") {
					out = arg;
                                }
                        } else {
                                std::cerr << "File path not given after '-o' option" << std::endl;
                                return 1;
                        }
                else {
                        in = arg;
                }
        }

	std::string cmd = "cc -E -x c ";

	if(in != "")
                cmd += in + " | ";

	cmd += getExecutableDir() + "/tokenizer ";

	cmd +=  " | " + getExecutableDir() + "/tokenparser ";


	if(out != "")
		cmd += "-o " + out;

	system(cmd.c_str());

	return 0;
}
