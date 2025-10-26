#pragma once

#include <iostream>

inline void unrecognizedTokenError(std::string file, size_t line, size_t column, std::string lines) {
        std::cerr << "In " << file << ";" << std::endl;
        std::cerr << "Unrecognized token at:" << line << ":" << column << std::endl;
	std::cerr << "\t" << lines << std::endl;
}

inline void unexpectedTokenError(Token &t) {
	std::cerr << "In the file;" << std::endl;
	std::cerr << "Unexpected token <" << t.name << "> at:" << t.line << ":" << t.column << std::endl;
}

inline void unclosedParanthesesError(Token &t) {
	if(t.ttype == Tokens::TOK_SYS_EOF)
		std::cout << "Expected ' ) ' but reached end of file!" << std::endl;
	else
		std::cout << "Expected ' ) ' but got ' " << t.name << " ' at: " << t.line << ":" << t.column << std::endl;
}

inline void unclosedSquareBracketsError(Token &t) {
        if(t.ttype == Tokens::TOK_SYS_EOF)
                std::cout << "Expected ' ] ' but reached end of file!" << std::endl;
        else
                std::cout << "Expected ' ] ' but got ' " << t.name << " ' at: " << t.line << ":" << t.column << std::endl;
}

inline void moreThanOneIdentifiersError(Token &t) {
	std::cerr << "More than one identifiers ' " << t.name << " ' at:" << t.line << ":" << t.column << std::endl;
}
