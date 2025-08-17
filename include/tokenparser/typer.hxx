typedef enum {
	VAR_ANGLE = 0,
	VAR_BIT,
	VAR_CHAR,
	VAR_SHORT,
	VAR_INT,
	VAR_FLOAT,
	VAR_DOUBLE,
	VAR_LONG,


	VAR_TYPES_COUNT
} VariableType;

enum {
	SPEC_QU = 0,
	SPEC_CONST = 0b10,
	SPEC_INL = 0b100,
	SPEC_EXT = 0b1000,
	SPEC_VOL = 0b10000
};

struct Typer {
	VariableType vtype;
	uint8_t spec;
};
