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

	uint8_t getPVarT(const std::string& str) {
		if(str == "angle")
			return VAR_ANGLE;
		else if(str == "bit")
			return VAR_BIT;
		else if(str == "char")
			return VAR_CHAR;
		else if(str == "short")
			return VAR_SHORT;
		else if(str == "int")
			return VAR_INT;
		else if(str == "float")
			return VAR_FLOAT;
		else if(str == "double")
			return VAR_DOUBLE;
		else if(str == "long")
			return VAR_LONG;
	}

	int eatTyper(Typer c_typer) {
		c_typer.vtype = VAR_UNDEFINED;
		c_typer.spec = char(0);
		for(int i=0; i < SPEC_COUNT; i++) {
			std::string _name=c_token.name;
			if(eat(Tokens::TOK_TYPE)) {
				--i;
				uint8_t vtype = getPVarT(_name);
                                if(vtype == VAR_LONG) switch(c_typer.vtype) {
					case VAR_LONG:
					case VAR_DOUBLE:
					case VAR_INT:
						++c_typer.vtype;
						continue;
					default: break;
				}
				if(c_typer.vtype)
					std::cerr << "Warning! Declaration with multiple types, last one will be count!!" << std::endl;
				c_typer.vtype = vtype;
			}
			else if(eat(Tokens::TOK_KEY_QUANTUM))
				c_typer.spec |= (1 << SPEC_QU);
			else if(eat(Tokens::TOK_KEY_CONST))
				c_typer.spec |= (1 << SPEC_CONST);
			else if(eat(Tokens::TOK_KEY_INLINE))
				c_typer.spec |= (1 << SPEC_INL);
			else if(eat(Tokens::TOK_KEY_EXTERN))
				c_typer.spec |= (1 << SPEC_EXT);
			else if(eat(Tokens::TOK_KEY_VOLATILE))
				c_typer.spec |= (1 << SPEC_VOL);
			else break;
		};
		return c_typer.spec || c_typer.vtype;
	};

	int getPSize(std::string s) {
		return 0;
	}

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

			Typer m_typer;
			if(eatTyper(m_typer)) {
				//eatIdtf or throw error
			}
		}

		return 0;
	}

	std::vector<StmPtr> main_block;

	int proc() {
		return proc(main_block);
	}
}
