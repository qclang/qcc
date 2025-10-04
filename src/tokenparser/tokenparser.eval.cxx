#include <tokenparser/tokenparser.hxx>
#include <tokenparser/expressions.hxx>

namespace Tokenparser {


	extern Token c_token;

        ExprPtr eval(Tokens::Type till) {
		if(c_token.ttype == Tokens::TOK_IDENTIFIER) {
			std::string name = c_token.name;
			eat(Tokens::TOK_IDENTIFIER);
			if(eat(Tokens::TOK_ASSIGN))
				return std::make_shared<BinaryExpression>(
							std::make_shared<VariableExpression>(name),
							OPE::ASSIGN,
							eval(Tokens::TOK_SEMICOLON));
		} else if(c_token.ttype == Tokens::TOK_NUMBER_LITERAL) {
			std::string name = c_token.name;
			eat(Tokens::TOK_NUMBER_LITERAL);

			return std::make_shared<LiteralExpression>(LIT_NUMBER, name);
		}

		return 0;
	}

}
