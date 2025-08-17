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

struct Typer {
	VariableType vtype;
        bool quantumised;
        bool constant;
        bool inlined;
        bool external;
        bool volatiled;
};
