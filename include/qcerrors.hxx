#include <iostream>

inline void unrecognizedTokenError(std::string file, size_t line, size_t column, std::string lines) {
        std::cerr << "In " << file << ";" << std::endl;
        std::cerr << "Unrecognized token at:" << line << ":" << column << std::endl;
	std::cerr << "\t" << lines << std::endl;
};

inline void unexpectedTokenError(Token& t) {
	std::cerr << "In the file;" << std::endl;
	std::cerr << "Un-expected token <" << t.name << "> at:" << t.line << ":" << t.column << std::endl;
};
