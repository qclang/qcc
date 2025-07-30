#pragma once

#include<unordered_map>
#include<vector>
#include<istream>
#include<tokenizer/tokens.hxx>

namespace Tokenizer {

	extern std::vector<Token> tokens;

	int proc(char* data, size_t length);

}
