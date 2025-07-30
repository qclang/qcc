#include <tokens.hxx>
#include <tokenizer.hxx>
#include <string>
#include <cstring>

#include <iostream>

namespace Tokenizer {

std::vector<Token> tokens;

size_t index, line, line_beg;

Token readAlpNum(char* data, size_t& index);
Token readNum(char* data, size_t& index);
Token readChar(char* data, size_t& index);
Token readString(char* data, size_t& index);
Token readSymbol(char* data, size_t& index);

void procAlpNum(Token& arg);
void procNum(Token& arg);
void procChar(Token& arg);
void procString(Token& arg);
void procSymbol(Token& arg);

bool isSymbol(char* data, size_t& index);

void proc(char *data, size_t length) {
	index = line_beg = 0;
	line = 1;
	char curr, peek;

        while(index < length) {
		curr = data[index];
		peek = data[index + 1];
                if((curr <= 'Z' && curr >= 'A') || ('a' <= curr && curr <= 'z') || curr == '_') { // Identifiers/keyword must start with an alphabetic char
			Token idtf = readAlpNum(data, index);
			procAlpNum(idtf);
		} else if(('0' <= curr && curr <= '9') || curr == '.') { // Number literal
			Token num = readNum(data, index);
		} else if(curr == '\'') { // Character literal
			Token c = readChar(data, index);
		} else if(curr == '"') { // String literal
			Token s = readString(data, index);
		} else if(isSymbol(data, index)) {
			Token s = readSymbol(data, index);
			procSymbol(s);
		} else if(curr == '\n') {
			++line;
			line_beg = ++index;
		} else if(curr == '\0') break;
        }
}

void procAlpNum(Token& arg) {
	const std::string& name = arg.name;
	Tokens::Type type;
	if(name == "angle" ||
		name == "qubit" ||
		name == "bit" ||
		name == "int" ||
		name == "float")
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
		type == Tokens::TOK_KEY_RETURN;
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
		type = Tokens::TOK_SYM_EQ;
	else if(name == "+")
                type = Tokens::TOK_SYM_PLUS;
	else if(name == "-")
                type = Tokens::TOK_SYM_MINUS;
	else if(name == "*")
                type = Tokens::TOK_SYM_STAR;
	else if(name == "/")
                type = Tokens::TOK_SYM_SLASH;
	else if(name == ":")
                type = Tokens::TOK_SYM_COLON;
	else if(name == ";")
                type = Tokens::TOK_SYM_SEMICOLON;
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
                type = Tokens::TOK_OP_NEG;
	else if(name == "<<")
		type = Tokens::TOK_OP_SHL;
	else if(name == ">>")
                type = Tokens::TOK_OP_SHR;
	else if(name == ",")
                type = Tokens::TOK_COMMA;
	else if(name == ".")
                type = Tokens::TOK_DOT;

	arg.ttype = type;
};

Token readAlpNum(char* data, size_t& index) {
	size_t end=index;
	while((data[end] <= 'Z' && data[end] >= 'A') || (data[end] <= 'z' && data[end] >= 'a') || (data[end] <= '9' && data[end] >= '0') || data[end] == '_')
		++end;
	size_t len = end - index + 1;

	char *buff = new char[len + 1];
	std::memcpy(buff, &data[index], len);
	buff[len] = '\0';
	std::string ret =  buff;
	delete[] buff;

	Token tok;
	tok.name = ret;
	tok.line = line;
	tok.column = end - line_beg + 1;
	tok.startOffset = index;
	tok.endOffset = end;

	index=end;
	return tok;
}

Token readNum(char* data, size_t& index) {
        size_t end=index;
        while((data[end] <= '9' && data[end] >= '0') || data[end] == '_' || data[end] == '.')
                ++end;
        size_t len = end - index + 1;

        char *buff = new char[len + 1];
        std::memcpy(buff, &data[index], len);
	buff[len] = '\0';
        std::string ret = buff;
	delete[] buff;

	Token tok;
	tok.name = ret;
        tok.line = line;
        tok.column = end - line_beg + 1;
        tok.startOffset = index;
        tok.endOffset = end;
	tok.ttype = Tokens::TOK_NUMBER_LITERAL;

	index=end;
	return tok;
}

Token readChar(char* data, size_t& index) {
	size_t end = ++index;
	char c;
	if(data[end] == '\\') {
		switch(++end) {
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
	} else c = data[end];

	Token tok;
	tok.name = c;
        tok.line = line;
        tok.column = end - line_beg + 1;
        tok.startOffset = index;
        tok.endOffset = end;
	tok.ttype = Tokens::TOK_CHAR_LITERAL;

	index=end;
	return tok;
}

Token readString(char* data, size_t& index) {
	size_t end=++index;
	while(data[end] != '"') ++end;

	size_t len = end - index + 1;
	end=index;

	while(data[end] != '"')
		if(data[end++] == '\\')
			--len;

	end=index;

	size_t pad=0;
	char *buff = new char[len + 1];

	while(data[end] != '"' && data[end] != '\0') {
		size_t _pos = end - index - pad;
		if(data[index] == '\\') {
			++pad;
	                switch(++index) {
                        	case 'n':
                	                buff[_pos] = '\n';
					break;
        	                case 't':
	                                buff[_pos] = '\t';
					break;
                        	case '0':
                	                buff[_pos] =  '\0';
					break;
				case '\\':
                                        buff[_pos] =  '\\';
					break;
				case '"':
					buff[_pos] = '"';
					break;
        	        }
	        } else buff[_pos] = data[index];
		++index;
	}

	buff[len] = '\0';
        std::string ret =  buff;
	delete[] buff;


	Token tok;
	tok.name = ret;
        tok.line = line;
        tok.column = end - line_beg + 1;
        tok.startOffset = index;
        tok.endOffset = end;
	tok.ttype = Tokens::TOK_STRING_LITERAL;

	index=end;
	return tok;
}

Token readSymbol(char* data, size_t& index) {
	Token tok;
        tok.name = data[index];
        tok.line = line;
        tok.column = index - line_beg + 1;
        tok.startOffset = tok.endOffset = index;

	if(isSymbol(data, ++index)) {
		tok.line += data[index++];
		++tok.endOffset;
	}

	return tok;
}

bool isSymbol(char* data, size_t& index) {
	char& c = data[index],
		p = data[index + 1];
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
