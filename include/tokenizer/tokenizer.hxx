#pragma once

#include<unordered_map>
#include<vector>
#include<istream>
#include<tokenizer/tokens.hxx>

namespace Tokenizer {

	extern std::vector<Token> tokens;

	void use(std::istream &input_stream);
	void use(std::ostream &output_stream);

	int proc();
}
