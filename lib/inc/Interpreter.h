#pragma once

#include "Parser.h"

namespace seal {
	namespace impl {
		class Interpreter {
		public:
			Interpreter(Context cont);
			std::optional<Value> VisitStatementList(StatementList& sl);
			std::optional<Value> VisitStatement(Statement& sl);
			std::optional<Value> VisitWhileLoop(WhileLoop& wl);
			std::optional<Value> VisitIfStatement(IfStatement& is);
			std::optional<Value> VisitPrintStatement(PrintStatement& ps);
			std::optional<Value> VisitAssignmentStatement(AssignmentStatement& as);
			std::optional<Value> VisitReadStatement(ReadStatement& rs);
			std::optional<Value> VisitExpression(Expression& e);
			std::optional<Value> VisitLogicsExpression(LogicsExpression& le);
			std::optional<Value> VisitAdditive(Additive& a);
			std::optional<Value> VisitMultiplicative(Multiplicative& m);
			std::optional<Value> VisitUnary(Unary& u);
			std::optional<Value> VisitFactor(Factor& f);
			std::optional<Value> VisitBrackets(Brackets& b);
			std::optional<Value> VisitLiteral(Literal& l);
			std::optional<Value> VisitIdentificator(Identificator& i);
		private:
			std::vector<Context> context;
			ErrorManager em;
		};
	}
}
