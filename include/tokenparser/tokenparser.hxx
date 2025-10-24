#pragma once

#include<iostream>
#include<vector>
#include<tokenizer/tokens.hxx>
#include<tokenparser/expressions.hxx>

namespace Tokenparser {

	void use(std::istream &input_stream);
	void use(std::shared_ptr<BlockStatement> stm_root);

	ExprPtr eval_single(Tokens::Type till);
	std::shared_ptr<TupleExpression> eval(Tokens::Type till);

	int eat(Tokens::Type ttype);
	int eatDec(std::shared_ptr<Typer> main_typer, std::vector<StmPtr> *parent);

        int proc(std::shared_ptr<BlockStatement> parent, const bool _inline = true);
        int proc_body(std::shared_ptr<BlockStatement> parent, Tokens::Type end_token = Tokens::TOK_SYS_EOF);
	int proc();
}
