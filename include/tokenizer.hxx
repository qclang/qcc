#pragma once

#include<unordered_map>
#include<vector>
#include<istream>


namespace Tokenizer {

	bool line_beg;

	std::vector<Token> tokenizer;
	std::unordered_map<std::string, std::string> definitions;

	void proc(char* data, size_t length);

	void eval_macro(char* data, size_t& index);

	std::string readAlpNum(char* data, size_t& index&);
	std::string readNum(char* data, size_t& index&);
	std::string readChar(char* data, size_t& index&);
	std::string readString(char* data, size_t& index&);

	void procAlpNum(std::string& s);
	void procNum(std::string& s);
	void procChar(char& c);
	void procString(std::string& s);

}
