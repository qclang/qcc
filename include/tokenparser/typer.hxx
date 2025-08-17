typedef enum {
	VAR_ANGLE = 0,
	VAR_BIT,
	VAR_CHAR,
	VAR_SHORT,
	VAR_INT,
	VAR_LINT,
	VAR_FLOAT,
	VAR_DOUBLE,
	VAR_LDOUBLE,
	VAR_LONG,
	VAR_LLONG
} VariableType;

enum {
	SPEC_QU = 0,
	SPEC_CONST,
	SPEC_INL,
	SPEC_EXT,
	SPEC_VOL,


	SPEC_COUNT
};

struct Typer {
	VariableType vtype;
	uint8_t spec;
};
