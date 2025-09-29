#include <tokenparser/tokenparser.hxx>
#include <tokenparser/expressions.hxx>
#include <qcerrors.hxx>

namespace Tokenparser {

	std::istream  *_input_stream;
	BlockStatement *stm_root;

	void use(std::istream &input_stream) {
	        _input_stream = &input_stream;
	}

	void use(BlockStatement* _stm_root) {
		stm_root = _stm_root;
	}

	Token c_token;

	inline int eat(Tokens::Type ttype) { // return 1 if match, 0 if not match

		if(c_token.ttype == ttype) {
			std::cout << "Eat: " << c_token.line << ":" << c_token.name << std::endl;
			_input_stream >> c_token;
			return 1;
		}

//		std::cout << "EX : " << ttype << std::endl;
//		std::cout << "Got: " << c_token.ttype << std::endl;
//		std::cout << "   : " << c_token.name << std::endl;

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


		std::shared_ptr<Typer> post_typer = nullptr;
		std::shared_ptr<Typer> c_post_typer = nullptr;

		while(true) {
			if(followAll && eat(Tokens::TOK_DEL_SBRACL)) {
				std::shared_ptr<Typer> arr_typer = std::make_shared<Typer>();
				arr_typer->vtype = VAR_ARRAY;
				if(!eat(Tokens::TOK_DEL_SBRACR)) /* Predict if is it already end */
					arr_typer->sizer = eval(/* Till that -> */ Tokens::TOK_DEL_SBRACR);

				if(!post_typer) post_typer = c_post_typer = arr_typer;
				else c_post_typer->respect_typer = arr_typer;

			} else if(followAll && eat(Tokens::TOK_DEL_PARANL)) {
				std::shared_ptr<Typer> func_typer = std::make_shared<Typer>();
				func_typer->vtype = VAR_FUN;
				if(!eat(Tokens::TOK_DEL_PARANR)) /* Predict if is it already end */
					eatDec(nullptr, &func_typer->func_params);

				if(!eat(Tokens::TOK_DEL_PARANR)) {
					/* Syntax error, S-Brackets ' [] ' didn't closed*/
				}

				if(!post_typer) post_typer = c_post_typer = func_typer;
				else c_post_typer->respect_typer = func_typer;

			} else break;
		}

		if(post_typer != nullptr) {
			post_typer->respect_typer = c_typer;
			c_typer = post_typer;
		}

		if(p_typer != nullptr) {
			p_typer->respect_typer = c_typer;
			c_typer = p_typer;
		}

		if(eat(Tokens::TOK_EQ)) {
			std::shared_ptr<Typer> fun_ptr = std::make_shared<Typer>();
			fun_ptr->vtype = VAR_POINTER;
			fun_ptr->respect_typer = c_typer;
			c_typer = fun_ptr;
		} else if(c_typer->vtype == VAR_FUN) {
			std::shared_ptr<BlockStatement> func_body = std::make_shared<BlockStatement>();

			proc(&func_body->childs);

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

		bool is_stm_open = true;
		do {
			std::shared_ptr<Typer> c_typer = std::make_shared<Typer>(*main_typer);
			if(!eatTyper(c_typer, true, parent)) {
				//error
				return 0;
			} else if(c_typer->vtype == VAR_DEC) {
				// Now declare/assign a variable with the variable/function 'c_typer->vname' (std::string)

				if(eat(Tokens::TOK_SEMICOLON)) {
					is_stm_open = false;
					break;
				}
			} else if(c_typer->vtype == VAR_FUN) {
				// ...
				is_stm_open = false;
			}
		} while(eat(Tokens::TOK_COMMA));

		if(is_stm_open) {
			/* Error */
			std::cout << "Expected a semicolon but get something else" << std::endl;
			return 0;
		}

		return 1;
	}



	/*
	* Return values and their meanings:
	*   0 : No catch
	*   1 : Problem caught
	*   2 : Finish
	*/

	int proc(std::vector<StmPtr> *parent, const bool _inline) {

		if(eat(Tokens::TOK_DEL_CBRACL)) {
			if(proc_body(parent)) return 1;

			if(!eat(Tokens::TOK_DEL_CBRACR)) {
				/* Error */
				std::cout << "Expected } but get something else" << std::endl;
				return 1;
			}
			return 0;
		}

		if(eatDec(nullptr, parent)) {
			if(_inline) {
				/* Warning */
				return 1;
			}
			return 0;
		}

		return 2;
	}

	int proc_body(std::vector<StmPtr> *parent) {
		while(!eat(Tokens::TOK_SYS_EOF)) {
			int cons = proc(parent, false);
			if(cons == 1)
				return 1;
			else if(cons == 2) return 0;
		}
		return 0;
	}

	int proc() {
		if(!_input_stream || !stm_root) return 1;
		_input_stream >> c_token;
		return proc_body(&stm_root->childs);
	}
}
