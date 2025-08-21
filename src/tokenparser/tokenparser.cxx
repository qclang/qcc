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
			std::cout << "Eat: " << c_token.line << ":" << c_token.name << std::endl;
			_input_stream >> c_token;
			return 1;
		}

		return 0;
	}

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
		return VAR_UNDEFINED;
	}

	ExprPtr eval(Tokens::Type till);

	int eatTyper(std::shared_ptr<Typer>& c_typer, bool followAll, std::vector<StmPtr>* parent = nullptr) {
		while(1) {
			if(c_token.ttype == Tokens::TOK_TYPE) {
				uint8_t vtype = getPVarT(c_token.name);
				eat(Tokens::TOK_TYPE);
                                if(vtype == VAR_LONG) switch(c_typer->vtype) {
					case VAR_LONG:
					case VAR_DOUBLE:
					case VAR_INT:
						++c_typer->vtype;
						continue;
					default: break;
				}
				if(c_typer->vtype)
					std::cerr << "Warning! Declaration with multiple types, last one will be count!!" << std::endl;
				c_typer->vtype = vtype;
			}
			else if(eat(Tokens::TOK_KEY_QUANTUM))
				c_typer->spec |= (1 << SPEC_QU);
			else if(eat(Tokens::TOK_KEY_CONST))
				c_typer->spec |= (1 << SPEC_CONST);
			else if(eat(Tokens::TOK_KEY_INLINE))
				c_typer->spec |= (1 << SPEC_INL);
			else if(eat(Tokens::TOK_KEY_EXTERN))
				c_typer->spec |= (1 << SPEC_EXT);
			else if(eat(Tokens::TOK_KEY_VOLATILE))
				c_typer->spec |= (1 << SPEC_VOL);
			else if(followAll && eat(Tokens::TOK_STAR)) {
				std::shared_ptr<Typer> ptr_typer = std::make_shared<Typer>();
				ptr_typer->vtype = VAR_POINTER;
				ptr_typer->respect_typer = c_typer;
				c_typer = ptr_typer;
			} else if(followAll && eat(Tokens::TOK_DEL_PARANL) ) {
				if(!eatTyper(c_typer, true, parent)) {
					// Error
					return 0;
				}
				if(!eat(Tokens::TOK_DEL_PARANR)) {
					// Should match left paranthese with right one
					return 0;
				}
			} else if(parent && c_token.ttype == Tokens::TOK_IDENTIFIER) {
				std::cout << "Dec: " << c_token.name << std::endl;
				eat(Tokens::TOK_IDENTIFIER);
				return 1;
                        } else break;
		};
		return c_typer->vtype;
	};

	int getPSize(std::string s) {
		return 0;
	}

	ExprPtr evalSquares() {
		return nullptr;
	} // eval array sizer or indexer


	int eatDec(std::vector<StmPtr>& parent) {
		std::shared_ptr<Typer> main_typer = std::make_shared<Typer>();
		if(!eatTyper(main_typer, false))
			return 0;
		do {
			std::shared_ptr<Typer> c_typer = std::make_shared<Typer>(*main_typer);
			if(!eatTyper(c_typer, true, &parent)) {
				//error
				return 0;
			}
		} while(eat(Tokens::TOK_COMMA));
		return 1;
	}

	StmPtr evalFunc() {
		return nullptr;
	}

	int proc(std::vector<StmPtr> &parent) {
		if(!_input_stream) return 1;
		_input_stream >> c_token;
		while(!eat(Tokens::TOK_SYS_EOF)) {
			std::string _name = c_token.name;

			std::shared_ptr<Typer> m_typer;
			if(eatDec(parent)) continue;
		}

		return 0;
	}

	std::vector<StmPtr> main_block;

	int proc() {
		return proc(main_block);
	}
}
