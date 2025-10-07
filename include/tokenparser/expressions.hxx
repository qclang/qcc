#pragma once

#include <string>
#include <memory>
#include <vector>

#include <tokenparser/typer.hxx>

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

	ASSIGN, // =

	ADDR, // &x
	DEREF, // *x

	MEM, // x.y
	MEMP, // x->y

	// Unary Operators
	NEG, // ~x
	INCB, // ++x
	INCA, // x++
	DECB, // --x
	DECA, // x--

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

	void accept(ExpressionVisitor& v) override;
};

struct UnaryExpression : public Expression {
        ExprPtr te;
        OPE op;

        UnaryExpression(ExprPtr te, OPE op);

        void accept(ExpressionVisitor& v) override;
};


enum {
	LIT_NUMBER,
	LIT_FLOAT,
	LIT_STRING
};

struct LiteralExpression : public Expression {
	int ltype;
	std::string value;

	LiteralExpression(int ltype, std::string value);

        void accept(ExpressionVisitor& v) override;
};

struct VariableExpression : public Expression {
	std::string name;

	VariableExpression(std::string name);

        void accept(ExpressionVisitor& v) override;
};

struct Statement {
	virtual ~Statement() = default;
	virtual void accept(ExpressionVisitor& v) = 0;
};

using StmPtr = std::shared_ptr<Statement>;

struct DeclarationStatement : Statement {
	std::shared_ptr<Typer> type_spec;

	void accept(ExpressionVisitor& v) override;
};

struct BlockStatement : public Statement {
	std::vector<StmPtr> childs;

	void accept(ExpressionVisitor& v) override;
};

struct ExpressionStatement : public Statement {
	ExprPtr expr;

	void accept(ExpressionVisitor& v) override;
};

struct IfStatement : public Statement {
	ExprPtr condition;
	StmPtr body;

	void accept(ExpressionVisitor& v) override;
};

struct WhileStatement : public IfStatement {
	bool atleastonce = false; // If true, process it like do-while

	void accept(ExpressionVisitor& v) override;
};

struct ForStatement : public Statement {
	StmPtr init;
	ExprPtr condition;
	StmPtr update;
	StmPtr body;

	void accept(ExpressionVisitor& v) override;
};

struct ReturnStatement : public Statement {
	ExprPtr expr;


	void accept(ExpressionVisitor& v) override;
};

struct ExpressionVisitor {
	virtual void visit(BinaryExpression &e) = 0;
	virtual void visit(UnaryExpression &e) = 0;
	virtual void visit(LiteralExpression &e) = 0;
	virtual void visit(VariableExpression &e) = 0;

	virtual void visit(DeclarationStatement &s) = 0;
	virtual void visit(BlockStatement &s) = 0;
	virtual void visit(ExpressionStatement &s) = 0;
	virtual void visit(IfStatement &s) = 0;
	virtual void visit(WhileStatement &s) = 0;
	virtual void visit(ForStatement &s) = 0;
	virtual void visit(ReturnStatement &s) = 0;
};
