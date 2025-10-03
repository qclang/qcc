#include <tokenizer/tokens.hxx>
#include <tokenizer/tokenizer.hxx>
#include <string>
#include <cstring>
#include <cstdint>
#include <cctype>
#include <qcerrors.hxx>
#include <iostream>
#include <unordered_map>

namespace Tokenizer {

size_t line, line_beg;

bool isSystem;

std::istream  *_input_stream;
std::ostream *_output_stream;

std::string current_file;

std::string readLine(std::istream &in);

Token readAlpNum(std::istream &in);
Token readNum(std::istream &in);
Token readChar(std::istream &in);
Token readString(std::istream &in);
Token readSymbol(std::istream &in);

void procAlpNum(Token& arg);
void procNum(Token& arg);
void procChar(Token& arg);
void procString(Token& arg);
void procSymbol(Token& arg);

bool isSymbol(const char &c);

void use(std::istream &input_stream) {
	_input_stream = &input_stream;
}

void use(std::ostream &output_stream) {
	_output_stream = &output_stream;
}

int proc() {
	std::istream& in = *_input_stream;

	line_beg = 0;
	line = 1;
	char curr;

        while(in && in.peek() != EOF) {
		curr = in.peek();
		Token c_token;

                if(curr == '#') {
			isSystem = false;
			in.ignore();
			in >> line;
			in.ignore();

			if(in.peek() == '"') {
				in.ignore();
				current_file = "";
				while(in.peek() != '"')
					current_file += in.get();
				in.ignore();
			}

			while(in.peek() != '\n') {
				int op;
				in >> op;
				// if(op == 1) std::cerr << "New file" << std::endl;
				// if(op == 2) std::cerr << "Finished" << std::endl;
				if(op == 3) isSystem = true;
				// if(op == 4) std::cerr << "Extern 'C'" << std::endl;
			};
			in.ignore();
			continue;
		} else if(std::isalnum(curr) || curr == '_') { // Identifiers/keyword must start with an alphabetic char or an '_' char
			c_token = readAlpNum(in);
			procAlpNum(c_token);
		} else if(std::isdigit(curr) || curr == '.') { // Number literal
			c_token = readNum(in);
		} else if(curr == '\'') { // Character literal
			c_token = readChar(in);
		} else if(curr == '"') { // String literal
			c_token = readString(in);
		} else if(isSymbol(curr)) {
			c_token = readSymbol(in);
			procSymbol(c_token);
		} else if(curr == '\n' || curr == ' ' || curr == '\t') {
			size_t start_index = static_cast<size_t>(in.tellg());
			size_t start_col = start_index - line_beg + 1;
			int curr = in.peek();

			while (curr == ' ' || curr == '\t' || curr == '\n') {
				in.get();
				if (curr == '\n') {
					++line;
					line_beg = static_cast<size_t>(in.tellg());
				}
				curr = in.peek();
			}

			continue;
		} else if(curr == '\0') {
			return 0;
		} else return( unrecognizedTokenError("the file",
						line,
						static_cast<size_t>(in.tellg()) - line_beg + 1,
						readLine(in)), 1);

		_output_stream << c_token;
        }

	Token eof_token;
	eof_token.ttype = Tokens::TOK_SYS_EOF;
	_output_stream << eof_token;

	return 0;
}

std::string readLine(std::istream &in) {
	std::string line;

	std::getline(in, line);

	return line;
}

void procAlpNum(Token& arg) {
	const std::string& name = arg.name;
	Tokens::Type type;
	if(name == "angle" ||
		name == "bit" ||
		name == "bool" ||
		name == "char" ||
		name == "short" ||
		name == "int" ||
		name == "float" ||
		name == "double" ||
		name == "long")
		type = Tokens::TOK_TYPE;
	else if(name == "struct")
		type = Tokens::TOK_STRUCT;
	else if(name == "if")
		type = Tokens::TOK_KEY_IF;
	else if(name == "for")
                type = Tokens::TOK_KEY_FOR;
	else if(name == "while")
		type = Tokens::TOK_KEY_WHILE;
	else if(name == "return")
		type = Tokens::TOK_KEY_RETURN;
	else if(name == "else")
                type = Tokens::TOK_KEY_ELSE;
        else if(name == "do")
                type = Tokens::TOK_KEY_DO;
        else if(name == "switch")
                type = Tokens::TOK_KEY_SWITCH;
        else if(name == "case")
                type = Tokens::TOK_KEY_CASE;
        else if(name == "default")
                type = Tokens::TOK_KEY_DEFAULT;
        else if(name == "break")
                type = Tokens::TOK_KEY_BREAK;
        else if(name == "continue")
                type = Tokens::TOK_KEY_CONTINUE;
        else if(name == "goto")
                type = Tokens::TOK_KEY_GOTO;
        else if(name == "sizeof")
                type = Tokens::TOK_KEY_SIZEOF;
        else if(name == "typedef")
                type = Tokens::TOK_KEY_TYPEDEF;
        else if(name == "const")
                type = Tokens::TOK_KEY_CONST;
        else if(name == "volatile")
                type = Tokens::TOK_KEY_VOLATILE;
        else if(name == "extern")
                type = Tokens::TOK_KEY_EXTERN;
        else if(name == "static")
                type = Tokens::TOK_KEY_STATIC;
        else if(name == "register")
                type = Tokens::TOK_KEY_REGISTER;
        else if(name == "inline")
                type = Tokens::TOK_KEY_INLINE;
        else if(name == "enum")
                type = Tokens::TOK_KEY_ENUM;
        else if(name == "union")
                type = Tokens::TOK_KEY_UNION;
	else if(name == "quantum")
                type = Tokens::TOK_KEY_QUANTUM;
	else type = Tokens::TOK_IDENTIFIER;

	arg.ttype = type;
};
void procNum(Token& arg) {};
void procChar(Token& arg) {};
void procString(Token& arg) {};
void procSymbol(Token& arg) {
	const std::string& name = arg.name;
	Tokens::Type type;
	if(name == "(")
		type = Tokens::TOK_DEL_PARANL;
	else if(name == ")")
		type = Tokens::TOK_DEL_PARANR;
	else if(name == "{")
		type = Tokens::TOK_DEL_CBRACL;
	else if(name == "}")
		type = Tokens::TOK_DEL_CBRACR;
	else if(name == "[")
		type = Tokens::TOK_DEL_SBRACL;
	else if(name == "]")
                type = Tokens::TOK_DEL_SBRACR;
	else if(name == "=")
		type = Tokens::TOK_ASSIGN;
	else if(name == "+")
                type = Tokens::TOK_PLUS;
	else if(name == "-")
                type = Tokens::TOK_MINUS;
	else if(name == "*")
                type = Tokens::TOK_STAR;
	else if(name == "/")
                type = Tokens::TOK_SLASH;
	else if(name == ":")
                type = Tokens::TOK_COLON;
	else if(name == ";")
                type = Tokens::TOK_SEMICOLON;
        else if(name == "%")
                type = Tokens::TOK_MOD;
        else if(name == "?")
                type = Tokens::TOK_QUESTION;
	else if(name == "==")
                type = Tokens::TOK_OP_EQEQ;
	else if(name == "!=")
                type = Tokens::TOK_OP_NEQ;
	else if(name == "<")
                type = Tokens::TOK_OP_LT;
	else if(name == ">")
                type = Tokens::TOK_OP_RT;
	else if(name == "<=")
                type = Tokens::TOK_OP_LTE;
        else if(name == ">=")
                type = Tokens::TOK_OP_RTE;
	else if(name == "&&")
                type = Tokens::TOK_OP_AND;
        else if(name == "||")
                type = Tokens::TOK_OP_OR;
	else if(name == "!")
		type = Tokens::TOK_OP_NOT;
	else if(name == "&")
		type = Tokens::TOK_OP_BIT_AND;
	else if(name == "|")
                type = Tokens::TOK_OP_BIT_OR;
	else if(name == "^")
                type = Tokens::TOK_OP_XOR;
	else if(name == "~")
                type = Tokens::TOK_NEG;
	else if(name == "<<")
		type = Tokens::TOK_SHL;
	else if(name == ">>")
                type = Tokens::TOK_SHR;
        else if(name == "<>")
                type = Tokens::TOK_ASSIGN_SWAP;
	else if(name == ",")
                type = Tokens::TOK_COMMA;
	else if(name == ".")
                type = Tokens::TOK_DOT;
	else if(name == "++")
		type = Tokens::TOK_INC;
        else if(name == "--")
                type = Tokens::TOK_DEC;
        else if(name == "->")
                type = Tokens::TOK_ARROW;
        else if(name == "...")
                type = Tokens::TOK_ELLIPSIS;
        else if(name == "+=")
                type = Tokens::TOK_ASSIGN_PLUS;
        else if(name == "-=")
                type = Tokens::TOK_ASSIGN_MINUS;
        else if(name == "*=")
                type = Tokens::TOK_ASSIGN_MUL;
        else if(name == "/=")
                type = Tokens::TOK_ASSIGN_DIV;
        else if(name == "%=")
                type = Tokens::TOK_ASSIGN_MOD;
        else if(name == "&=")
                type = Tokens::TOK_ASSIGN_AND;
        else if(name == "|=")
                type = Tokens::TOK_ASSIGN_OR;
        else if(name == "^=")
                type = Tokens::TOK_ASSIGN_XOR;
        else if(name == "<<=")
                type = Tokens::TOK_ASSIGN_SHL;
        else if(name == ">>=")
                type = Tokens::TOK_ASSIGN_SHR;

	arg.ttype = type;
};

Token readAlpNum(std::istream &in) {
	size_t start_index=in.tellg();

	std::string ret;

	int curr = in.peek();
	while(std::isalnum(curr) || curr == '_') {
		ret += static_cast<char>(in.get());
		curr = in.peek();
	}

	Token tok;
	tok.name = ret;
	tok.line = line;
	tok.endOffset = start_index + ret.size();
	tok.column = tok.endOffset - line_beg + 1;
	tok.startOffset = start_index;

	return tok;
}

Token readNum(std::istream& in) {
	size_t start_index=in.tellg();

        std::string ret;

        int curr = in.peek();
        while(std::isxdigit(curr) || curr == '.' || curr == '_') {
		if(curr == '_') continue;
                ret += static_cast<char>(in.get());
                curr = in.peek();
        }

        Token tok;
        tok.name = ret;
        tok.line = line;
        tok.endOffset = start_index + ret.size();
        tok.column = tok.endOffset - line_beg + 1;
        tok.startOffset = start_index;
	tok.ttype = Tokens::TOK_NUMBER_LITERAL;

        return tok;}

Token readChar(std::istream& in) {
	size_t start_index = in.tellg();
	in.get();
	char c = in.peek();
	if(c == '\\') {
		in.get();
		c = in.peek();
		switch(c) {
			case 'n':
				c =  '\n';
				break;
			case 't':
				c = '\t';
				break;
			case '0':
				c = '\0';
				break;
			case '\\':
				c = '\\';
				break;
			case '\'':
                                c = '\'';
				break;
		}
	}
	in.get();

	Token tok;
	std::string(1, static_cast<char>(c));
        tok.line = line;
        tok.column = static_cast<size_t>(in.tellg()) - line_beg + 1;
        tok.startOffset = start_index;
        tok.endOffset = static_cast<size_t>(in.tellg());
	tok.ttype = Tokens::TOK_CHAR_LITERAL;

	return tok;
}

Token readString(std::istream& in) {
	size_t start_index = static_cast<size_t>(in.tellg());
	std::string ret;

	in.get(); // opening '"'

	while (true) {
	int ch = in.get();
	if (ch == EOF || ch == '"') break;

	if (ch == '\\') {
		ch = in.get(); // next escaped char
		switch (ch) {
			case 'n':
				ret += '\n';
				break;
			case 't':
				ret += '\t';
				break;
			case '0':
				ret += '\0';
				break;
			case '\\':
				ret += '\\';
				break;
			case '"':
				ret += '"';
				break;
			default:
				ret += '\\';
				if (ch != EOF) ret += static_cast<char>(ch);
				break;
			}
		} else {
			ret += static_cast<char>(ch);
		}
	}

	size_t end_index = static_cast<size_t>(in.tellg());

	Token tok;
	tok.name = ret;
	tok.line = line;
	tok.column = start_index - line_beg + 1;
	tok.startOffset = start_index;
	tok.endOffset = end_index;
	tok.ttype = Tokens::TOK_STRING_LITERAL;

	return tok;
}


Token readSymbol(std::istream& in) {
	Token tok;
        tok.startOffset = tok.endOffset = in.tellg();
        tok.line = line;
        tok.column = tok.startOffset - line_beg + 1;

	char p = in.get();
        tok.name = std::string(1, p);

	if(p == '+') {
		p = in.peek();
		switch(p) {
			case '+':
			case '=':
			tok.name += in.get();
			break;
		}
	} else if(p == '-') {
                p = in.peek();
                switch(p) {
                        case '-':
			case '>':
                        case '=':
                        tok.name += in.get();
                        break;
                }
        } else switch(p) {
		case '*':
		case '/':
		case '%':
		case '^':
        	        p = in.peek();
			if(p == '=')
                        	tok.name += in.get();
			break;
		case '&':
                        p = in.peek();
                        if(p == '&' || p == '=')
                                tok.name += in.get();
                        break;
                case '|':
                        p = in.peek();
                        if(p == '|' || p == '=')
                                tok.name += in.get();
                        break;
		case '.':
                        p = in.peek();
                        if(p == '.') {
                                tok.name += in.get();
				if(p == '.')
                                	tok.name += in.get();
			}
                        break;
		case '<':
                        p = in.peek();
                        if(p == '<') {
                                tok.name += in.get();
                                if(p == '=')
                                        tok.name += in.get();
                        } else if(p == '>') tok.name += in.get();
                        break;
		case '>':
                        p = in.peek();
                        if(p == '>') {
                                tok.name += in.get();
                                if(p == '=')
                                        tok.name += in.get();
                        }
                        break;
        }

	return tok;
}

bool isSymbol(const char &c) {
	switch(c) {
		case '(':
		case ')':
		case '{':
		case '}':
		case '[':
		case ']':
		case '=':
		case '+':
		case '-':
		case '*':
		case '/':
		case ':':
		case ';':
		case '!':
		case '<':
		case '>':
		case '&':
		case '|':
		case '^':
		case '~':
		case '.':
		case ',':
			return true;
	}
	return false;
};

}
