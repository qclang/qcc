#include <memory>

enum {
	VAR_UNDEFINED=0,
	VAR_ANGLE,
	VAR_BIT,
	VAR_CHAR,
	VAR_SHORT,
	VAR_INT,
	VAR_LINT,
	VAR_FLOAT,
	VAR_DOUBLE,
	VAR_LDOUBLE,
	VAR_LONG,
	VAR_LLONG,
	VAR_POINTER
};

enum {
	SPEC_QU = 0,
	SPEC_CONST,
	SPEC_INL,
	SPEC_EXT,
	SPEC_VOL,


	SPEC_COUNT
};

struct Typer {
	uint8_t vtype;
	uint8_t spec;
	std::shared_ptr<Typer> respect_ptr;
};
