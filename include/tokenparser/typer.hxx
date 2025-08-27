#pragma once

#include <memory>
#include <string>
#include <cstdint>

enum {
	VAR_UNDEFINED=0,
	VAR_ANGLE,
	VAR_BIT,
	VAR_BOOL,
	VAR_CHAR,
	VAR_SHORT,
	VAR_FLOAT,
	VAR_DOUBLE,
	VAR_LDOUBLE,
	VAR_INT,
	VAR_LONG,
	VAR_LLONG,
	VAR_POINTER,
	VAR_ARRAY,
	VAR_DEC
};

enum {
	SPEC_QU = 0,
	SPEC_CONST,
	SPEC_INL,
	SPEC_EXT,
	SPEC_VOL,
};

struct Expression; // Pre definition
using ExprPtr = std::shared_ptr<Expression>;

struct Typer {
	std::string var_name;
	ExprPtr sizer;
	uint8_t vtype;
	uint8_t spec;
	std::shared_ptr<Typer> respect_typer;
};
