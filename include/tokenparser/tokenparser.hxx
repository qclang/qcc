#pragma once

#include<iostream>
#include<tokenizer/tokens.hxx>
#include<tokenparser/expressions.hxx>

namespace Tokenparser {

	void use(std::istream &input_stream);
	void use(BlockStatement* stm_root);

	int proc();
}
