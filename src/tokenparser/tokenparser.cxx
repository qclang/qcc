#include <tokenparser/tokenparser.hxx>
#include <tokenparser/expressions.hxx>
#include <qcerrors.hxx>

namespace Tokenparser {

	std::istream  *_input_stream;
	std::ostream *_output_stream;

	void use(std::istream &input_stream) {
	        _input_stream = &input_stream;
	}

	void use(std::ostream &output_stream) {
	        _output_stream = &output_stream;
	}

	Token c_token;

	int eat(Tokens::Type ttype) { // return 1 if match, 0 if not match
		if(c_token.ttype == ttype) {
			_input_stream >> c_token;
			return 1;
		}

		return 0;
	};

	int getPSize(std::string s) {
		return 0;
	};

	ExprPtr evalSquares() {
		return nullptr;
	} // eval array sizer or indexer

	void evalDec() {} // Return declarationstatement

	StmPtr evalFunc() {
		return nullptr;
	}

	int proc(std::vector<StmPtr> &parent) {
		while(_input_stream && _input_stream->peek() != EOF) {
			_input_stream >> c_token;
			std::string _name = c_token.name;

			if(eat(Tokens::TOK_TYPE)) {
				ExprPtr sizer;
				sizer = eat(Tokens::TOK_DEL_SBRACL) ?
					evalSquares() :
					std::make_shared<LiteralExpression>(std::to_string(getPSize(_name)));

				bool isPtr= eat(Tokens::TOK_STAR);

				if(eat(Tokens::TOK_IDENTIFIER)) {
					
					return 1;
				}

				if(eat(Tokens::TOK_DEL_PARANL)) {
					evalFunc();
					continue;
				}

				if(eat(Tokens::TOK_SEMICOLON)) {
					
					continue;
				}

				if(eat(Tokens::TOK_EQ)) {
					//dec
				}

				if(eat(Tokens::TOK_COMMA)) {
					evalDec();
					continue;
				}
			}
		}

		return 0;
	}

	std::vector<StmPtr> main_block;

	int proc() {
		return proc(main_block);
	}
}
