#include <tokenparser/expressions.hxx>

BinaryExpression::BinaryExpression(ExprPtr left, OPE op, ExprPtr right) : left(left), op(op), right(right) {}
UnaryExpression::UnaryExpression(ExprPtr te, OPE op) : te(te), op(op) {}
LiteralExpression::LiteralExpression(std::string value) : value(value) {}
VariableExpression::VariableExpression(std::string name) : name(name) {}

void BinaryExpression::accept(ExpressionVisitor& v) { v.visit(*this); }
void UnaryExpression::accept(ExpressionVisitor& v) { v.visit(*this); }
void LiteralExpression::accept(ExpressionVisitor& v) { v.visit(*this); }
void VariableExpression::accept(ExpressionVisitor& v) { v.visit(*this); }
