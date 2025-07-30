#pragma once

#include <tokenizer/variables.hxx>

#include <cstddef>
#include <string>

namespace Tokens {

typedef enum {
	TOK_TYPE,
	TOK_STRUCT,
	TOK_IDENTIFIER,
	TOK_NUMBER_LITERAL,
	TOK_CHAR_LITERAL,
	TOK_STRING_LITERAL,

	TOK_WHITESPACE,

	TOK_KEY_IF,
	TOK_KEY_FOR,
	TOK_KEY_WHILE,

	TOK_KEY_SWAP,

	TOK_DEL_PARANL,  // (
	TOK_DEL_PARANR,  // )
	TOK_DEL_CBRACL,  // {
	TOK_DEL_CBRACR,  // }
	TOK_DEL_SBRACL,  // [
	TOK_DEL_SBRACR,  // ]

	TOK_SYM_EQ, // =
	TOK_SYM_PLUS, // +
	TOK_SYM_MINUS, // -
	TOK_SYM_STAR, // *
	TOK_SYM_SLASH, // /
	TOK_SYM_COLON,  // :
	TOK_SYM_SEMICOLON, // ;

	TOK_OP_EQEQ, // ==
	TOK_OP_NEQ, // !=
	TOK_OP_LT, // <
	TOK_OP_RT, // >
	TOK_OP_LTE, // <=
	TOK_OP_RTE, // >=

	TOK_OP_AND,      // &&
	TOK_OP_OR,       // ||
	TOK_OP_NOT,      // !

	TOK_OP_BIT_AND, // &
	TOK_OP_BIT_OR, // |
	TOK_OP_XOR, // ^
	TOK_OP_NEG, // ~
	TOK_OP_SHL, // <<
	TOK_OP_SHR, // >>

	TOK_COMMA,       // ,
	TOK_DOT,         // .

	TOK_KEY_RETURN
} Type;

}

struct Token {
	Tokens::Type ttype;

	std::string name;

	int bits;

	int line = 0;
	int column = 0;
	int startOffset = 0;
	int   endOffset = 0;

	bool isSynthetic = false;

};
