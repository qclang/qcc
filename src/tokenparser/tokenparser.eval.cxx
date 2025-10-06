#include <tokenparser/tokenparser.hxx>
#include <tokenparser/expressions.hxx>

namespace Tokenparser {


	extern Token c_token;

	ExprPtr evalIDTF() {
		if(eat(Tokens::TOK_INC)) {

			ExprPtr idtf_expr = evalIDTF();

			if(!idtf_expr) return 0;

			return std::make_shared<UnaryExpression>(idtf_expr, OPE::INCB);
		} else if(eat(Tokens::TOK_DEC)) {

			ExprPtr idtf_expr = evalIDTF();

			if(!idtf_expr) return 0;

			return std::make_shared<UnaryExpression>(idtf_expr, OPE::DECB);
		} else if(c_token.ttype == Tokens::TOK_IDENTIFIER) {
			std::shared_ptr<VariableExpression> var_expr = std::make_shared<VariableExpression>(c_token.name);
			eat(Tokens::TOK_IDENTIFIER);

			if(eat(Tokens::TOK_INC))
				return std::make_shared<UnaryExpression>(var_expr, OPE::INCA);
			else if(eat(Tokens::TOK_DEC))
				return std::make_shared<UnaryExpression>(var_expr, OPE::DECA);


			return var_expr;
		}

		return 0;
	}

	ExprPtr eval(Tokens::Type till) {
		{
			ExprPtr idtf_expr = evalIDTF();
			if(idtf_expr) {
				if(eat(Tokens::TOK_ASSIGN))
					return std::make_shared<BinaryExpression>(
								idtf_expr,
								OPE::ASSIGN,
								eval(Tokens::TOK_SEMICOLON));
				else {
					std::shared_ptr c_expr = std::make_shared<ExpressionStatement>();
					c_expr->expr = idtf_expr;
				}
			}
		}



		if(c_token.ttype == Tokens::TOK_NUMBER_LITERAL) {
			std::string name = c_token.name;
			eat(Tokens::TOK_NUMBER_LITERAL);

			return std::make_shared<LiteralExpression>(LIT_NUMBER, name);
		}

		return 0;
	}

}
