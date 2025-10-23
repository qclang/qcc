#include <tokenparser/tokenparser.hxx>
#include <tokenparser/expressions.hxx>

namespace Tokenparser {


	extern Token c_token;

	ExprPtr evalIDTF() {
		if(eat(Tokens::TOK_INC)) {

			ExprPtr main_expr = evalIDTF();

			if(!main_expr) return 0;

			return std::make_shared<UnaryExpression>(main_expr, OPE::INCB);
		} else if(eat(Tokens::TOK_DEC)) {

			ExprPtr main_expr = evalIDTF();

			if(!main_expr) return 0;

			return std::make_shared<UnaryExpression>(main_expr, OPE::DECB);
		} else if(c_token.ttype == Tokens::TOK_IDENTIFIER) {
			std::shared_ptr<VariableExpression> var_expr = std::make_shared<VariableExpression>(c_token.name);
			eat(Tokens::TOK_IDENTIFIER);

			if(eat(Tokens::TOK_DEL_PARANL)) {
				std::shared_ptr<TupleExpression> tuple = eval(Tokens::TOK_DEL_PARANR);
				std::shared_ptr<UnaryExpression> func = std::make_shared<UnaryExpression>(var_expr, OPE::FUNCALL);
				func->tuple = tuple;
				return func;
			} else if(eat(Tokens::TOK_INC))
				return std::make_shared<UnaryExpression>(var_expr, OPE::INCA);
			else if(eat(Tokens::TOK_DEC))
				return std::make_shared<UnaryExpression>(var_expr, OPE::DECA);


			return var_expr;
		}

		return 0;
	}

	ExprPtr eval() {
		{
			ExprPtr main_expr = evalIDTF();
			if(main_expr) {
				if(eat(Tokens::TOK_ASSIGN))
					return std::make_shared<BinaryExpression>(
								main_expr,
								OPE::ASSIGN,
								eval(Tokens::TOK_SEMICOLON));
				else
					return main_expr;
			}
		}



		if(c_token.ttype == Tokens::TOK_NUMBER_LITERAL) {
			std::string name = c_token.name;
			eat(Tokens::TOK_NUMBER_LITERAL);

			return std::make_shared<LiteralExpression>(LIT_NUMBER, name);
		}

		return 0;
	}

	std::shared_ptr<TupleExpression> eval(Tokens::Type till) {
		std::shared_ptr<TupleExpression> tuple = std::make_shared<TupleExpression>();
		do {
			tuple->expressions.push_back(eval());
		} while(eat(Tokens::TOK_COMMA));
		return tuple;
	}
}
