#include <tokenparser/tokenparser.hxx>
#include <tokenparser/expressions.hxx>

namespace Tokenparser {


	extern Token c_token;

	ExprPtr eval3() {
		if(eat(Tokens::TOK_INC)) {

			ExprPtr main_expr = eval3();

			if(!main_expr) return 0; /* Error */

			return std::make_shared<UnaryExpression>(main_expr, OPE::INCB);
		} else if(eat(Tokens::TOK_DEC)) {

			ExprPtr main_expr = eval3();

			if(!main_expr) return 0; /* Error */

			return std::make_shared<UnaryExpression>(main_expr, OPE::DECB);
		} else if(c_token.ttype == Tokens::TOK_IDENTIFIER) {
			std::shared_ptr<VariableExpression> var_expr = std::make_shared<VariableExpression>(c_token.name);
			eat(Tokens::TOK_IDENTIFIER);

			if(eat(Tokens::TOK_DEL_SBRACL)) {
				ExprPtr param = eval(Tokens::TOK_DEL_SBRACR);
                                std::shared_ptr<UnaryExpression> member = std::make_shared<UnaryExpression>(var_expr, OPE::MEMA);
                                member->param = param;
				return member;
			} else if(eat(Tokens::TOK_DEL_PARANL)) {
				std::shared_ptr<TupleExpression> tuple = eval_tuple(Tokens::TOK_DEL_PARANR);
				std::shared_ptr<UnaryExpression> func = std::make_shared<UnaryExpression>(var_expr, OPE::FUNCALL);
				func->tuple = tuple;
				return func;
			} else if(eat(Tokens::TOK_INC))
				return std::make_shared<UnaryExpression>(var_expr, OPE::INCA);
			else if(eat(Tokens::TOK_DEC))
				return std::make_shared<UnaryExpression>(var_expr, OPE::DECA);


			return var_expr;
		}

		if(eat(Tokens::TOK_DEL_PARANL))
			return eval(Tokens::TOK_DEL_PARANR);

		if(eat(Tokens::TOK_DEL_SBRACL))
			return eval_tuple(Tokens::TOK_DEL_SBRACR);

		if(c_token.ttype == Tokens::TOK_STRING_LITERAL) {
			std::string name = c_token.name;
			eat(Tokens::TOK_STRING_LITERAL);

			return std::make_shared<LiteralExpression>(LIT_STRING, name);
		}

		if(c_token.ttype == Tokens::TOK_NUMBER_LITERAL) {
			std::string name = c_token.name;
			eat(Tokens::TOK_NUMBER_LITERAL);

			return std::make_shared<LiteralExpression>(LIT_NUMBER, name);
		}

		if(c_token.ttype == Tokens::TOK_CHAR_LITERAL) {
			std::string name = c_token.name;
			eat(Tokens::TOK_CHAR_LITERAL);

			return std::make_shared<LiteralExpression>(LIT_CHAR, name);
		}

		return 0;
	}

	ExprPtr eval2() {
		ExprPtr main_expr = eval3();
		if(!main_expr) return 0;

		if(eat(Tokens::TOK_STAR))
			return std::make_shared<BinaryExpression>(
						main_expr,
						OPE::MUL,
						eval2());
		else if(eat(Tokens::TOK_SLASH))
			return std::make_shared<BinaryExpression>(
						main_expr,
						OPE::DIV,
						eval2());
		else if(eat(Tokens::TOK_MOD))
			return std::make_shared<BinaryExpression>(
						main_expr,
						OPE::MOD,
						eval2());

		return main_expr;
	}

	ExprPtr eval1() {
		ExprPtr main_expr = eval2();
		if(!main_expr) return 0;

		if(eat(Tokens::TOK_PLUS))
			return std::make_shared<BinaryExpression>(
						main_expr,
						OPE::ADD,
						eval1());
		else if(eat(Tokens::TOK_MINUS))
			return std::make_shared<BinaryExpression>(
						main_expr,
						OPE::SUB,
						eval1());

		return main_expr;
	}

	ExprPtr eval() {
		ExprPtr main_expr = eval1();
		if(main_expr)
			if(eat(Tokens::TOK_ASSIGN))
				return std::make_shared<BinaryExpression>(
							main_expr,
							OPE::ASSIGN,
							eval());
		return 0;
	}

	ExprPtr eval_single(Tokens::Type till) {
		ExprPtr expr = eval(); // We'll modify that func later on
		if(!eat(till)) {
			/* Error */
			std::cout << "Unexpected token" << std::endl;
			return 0;
		}
		return expr;
	}

	std::shared_ptr<TupleExpression> eval_tuple(Tokens::Type till) {
		std::shared_ptr<TupleExpression> tuple = std::make_shared<TupleExpression>();
		do {
			ExprPtr expr = eval_single(Tokens::TOK_SYS_SKIP);
			if(expr)
				tuple->expressions.push_back(expr);
			else break;
		} while(eat(Tokens::TOK_COMMA));

		if(!eat(till)) { /* Error */
			std::cout << "Unexcepted token" << std::endl;
		}

		return tuple;
	}

	ExprPtr eval(Tokens::Type till) {
		ExprPtr main_expr = eval_single(Tokens::TOK_SYS_SKIP);
		if(main_expr) while(eat(Tokens::TOK_COMMA)) {
			ExprPtr expr = std::make_shared<BinaryExpression>(main_expr, OPE::COMMA, eval_single(Tokens::TOK_SYS_SKIP));
			if(expr)
				main_expr = expr;
			else break;
		}

		if(!eat(till)) { /* Error */
			std::cout << "Unexcepted token" << std::endl;
		}

		return main_expr;
	}
}
