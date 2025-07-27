#pragma once

#include<unordered_map>
#include<vector>
#include<istream>
#include<tokens.hxx>

namespace Tokenizer {

	extern std::vector<Token> tokens;

	void proc(char* data, size_t length);

	Token readAlpNum(char* data, size_t& index);
	Token readNum(char* data, size_t& index);
	Token readChar(char* data, size_t& index);
	Token readString(char* data, size_t& index);

	void procAlpNum(std::string& s);
	void procNum(std::string& s);
	void procChar(char& c);
	void procString(std::string& s);

}
