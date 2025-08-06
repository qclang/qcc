#include <string>

enum class OPE {
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,

	SHL,
	SHR,

	TER, // ternary, x ? 1 : 0

	EQEQ,
	AND,
	OR,
	ANDAND,
	OROR,

	XOR,

	SET, // =

	ADDR, // &x
	DEREF, // *x

	MEM, // x.y
	MEMP, // x->y

	// Unary Operators
	NEG, // ~x
	INCB, // ++x or x++
	INCL,
	DECC, // --x or x--
	DECL,

	NOT // !
};

struct ExpressionVisitor;

struct Expression {
	virtual ~Expression() = default;
	virtual void accept(ExpressionVisitor& v) = 0;
};

using ExprPtr = std::shared_ptr<Expression>;

struct BinaryExpression : public Expression {
	ExprPtr left, right;
	OPE op;

	BinaryExpression(ExprPtr left, OPE op, ExprPtr right);
};

struct UnaryExpression : public Expression {
        ExprPtr te;
        OPE op;

        BinaryExpression(ExprPtr te, OPE op);
};

struct LiteralExpression : public Expression {
	std::string value;

	LiteralExpression(std::string value);
};

struct VariableExpression : public Expression {
	std::string name;

	VariableExpression(std::string name);
};

struct ExpressionVisitor {
	virtual void visit(BinaryExpression &e) = 0;
	virtual void visit(UnaryExpression &e) = 0;
	virtual void visit(LiteralExpression &e) = 0;
	virtual void visit(VariableExpression &e) = 0;
};
