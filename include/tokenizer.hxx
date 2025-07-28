#pragma once

#include<unordered_map>
#include<vector>
#include<istream>
#include<tokens.hxx>

namespace Tokenizer {

	extern std::vector<Token> tokens;

	void proc(char* data, size_t length);

}
