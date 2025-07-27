#include <tokens.hxx>
#include <tokenizer.hxx>
#include <string>
#include <cstring>

#include <iostream>

namespace Tokenizer {

	std::vector<Token> tokens;

	size_t index, line, line_beg;

void proc(char *data, size_t length) {
	index = line_beg = 0;
	line = 1;
	char curr, peek;

        while(index < length) {
		curr = data[index];
		peek = data[index + 1];
                if((curr <= 'Z' && curr >= 'A') || ('a' <= curr && curr <= 'z')) { // Identifiers/keyword must start with an alphabetic char
			Token idtf = readAlpNum(data, index);
		} else if('0' <= curr && curr <= '9') { // Number literal
			Token num = readNum(data, index);
		} else if(curr == '\'') { // Character literal
			Token c = readChar(data, index);
		} else if(curr == '"') { // String literal
			Token s = readString(data, index);
		} else if(curr == '\n') {
			++line;
			line_beg = ++index;
		} else if(curr == '\0') break;
        }
}

void procAlpNum(Token& arg) {};
void procNum(Token& arg) {};
void procChar(Token& arg) {};
void procString(Token& arg) {};

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


	index=end;
	return tok;
}

}
