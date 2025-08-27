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
		else if(str == "bool")
			return VAR_BOOL;
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

	ExprPtr evalSquares() {
		std::cout << "Sizer!" << std::endl;
		return nullptr;
	} // eval array sizer or indexer

	int eatDec(std::shared_ptr<Typer> main_typer, std::vector<StmPtr>* parent = nullptr);

	int eatTyper(std::shared_ptr<Typer>& c_typer, bool followAll, std::vector<StmPtr>* parent = nullptr) {
		std::shared_ptr<Typer> p_typer = nullptr; // to add parantheses at the end, parantheses has more priority
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
				p_typer = std::make_shared<Typer>();
				if(!eatDec(p_typer, parent)) {
					// Error
					return 0;
				}
				if(!eat(Tokens::TOK_DEL_PARANR)) {
					// Should match left paranthese with right one
					return 0;
				}
				break;
			} else if(parent && c_token.ttype == Tokens::TOK_IDENTIFIER) {
				std::shared_ptr<Typer> dec_typer = std::make_shared<Typer>();
                                dec_typer->vtype = VAR_DEC;
				dec_typer->var_name = c_token.name;
                                dec_typer->respect_typer = c_typer;
                                c_typer = dec_typer;
				eat(Tokens::TOK_IDENTIFIER); // end
				break;
                        } else break;
		};

		if(followAll && eat(Tokens::TOK_DEL_SBRACL)) {
			std::shared_ptr<Typer> arr_typer = std::make_shared<Typer>();
			arr_typer->vtype = VAR_ARRAY;
			arr_typer->sizer = evalSquares();
			if(!eat(Tokens::TOK_DEL_SBRACR)) { /* error */}
		}

		if(p_typer != nullptr) {
			p_typer->respect_typer = c_typer;
			c_typer = p_typer;
		}

		return c_typer->vtype;
	};

	int getPSize(std::string s) {
		return 0;
	}

	int eatDec(std::shared_ptr<Typer> main_typer, std::vector<StmPtr>* parent) {
		if(!main_typer)
			main_typer = std::make_shared<Typer>();
		if(!eatTyper(main_typer, false))
			return 0;
		do {
			std::shared_ptr<Typer> c_typer = std::make_shared<Typer>(*main_typer);
			if(!eatTyper(c_typer, true, parent)) {
				//error
				return 0;
			} else if(c_typer->vtype == VAR_DEC) {
				// Now declare a variable with the variable 'to_be_declared_name' (std::string)
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

			if(eatDec(nullptr, &parent)) continue;
		}

		return 0;
	}

	std::vector<StmPtr> main_block;

	int proc() {
		return proc(main_block);
	}
}
