#pragma once

#include<iostream>
#include<tokenizer/tokens.hxx>
#include<tokenparser/expressions.hxx>

namespace Tokenparser {

	void use(std::istream &input_stream);
	void use(BlockStatement* stm_root);

	ExprPtr eval(Tokens::Type till);
	ExprPtr evalSquares();

	int eat(Tokens::Type ttype);
	int eatDec(std::shared_ptr<Typer> main_typer, std::vector<StmPtr>* parent);

	int proc();
}
