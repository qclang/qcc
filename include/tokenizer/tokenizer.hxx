#pragma once

#include<iostream>
#include<tokenizer/tokens.hxx>

namespace Tokenizer {

	void use(std::istream &input_stream);
	void use(std::ostream &output_stream);

	int proc();
}
