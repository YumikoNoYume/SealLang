#include "Interpreter.h"

namespace seal {
	namespace impl {
		Interpreter::Interpreter(Context cont) {
			if (!cont.variables.empty()) {
				context.push_back(cont);
			}
		}
		
		std::optional<Value> Interpreter::VisitStatementList(StatementList& sl) {
			Context con;
			context.push_back(con);
			for (auto& node : sl.node_list) {
				node->Accept(*this);
			}
			context.pop_back();
			if (em.GetErrorListSize() > 0) {
				throw (Exception(std::move(em.GetErrorMessages())));
			}
			return std::nullopt;
		}

		std::optional<Value> Interpreter::VisitStatement(Statement& s) {
			s.node->Accept(*this);
			return std::nullopt;
		}

		std::optional<Value> Interpreter::VisitWhileLoop(WhileLoop& wl) {
			while (wl.logics->Accept(*this).value().num_value) {
				wl.body->Accept(*this);
			}
			return std::nullopt;
		}

		std::optional<Value> Interpreter::VisitIfStatement(IfStatement& is) {
			if (is.logics->Accept(*this).value().num_value) {
				is.body->Accept(*this);
			}
			return std::nullopt;
		}

		std::optional<Value> Interpreter::VisitPrintStatement(PrintStatement& ps) {
			auto val = ps.variable->Accept(*this);
			std::cout << val.value().num_value << "\n";
			return std::nullopt;
		}

		std::optional<Value> Interpreter::VisitAssignmentStatement(AssignmentStatement& as) {
			auto val = as.expression->Accept(*this);
			for (std::size_t index = 0; index < context.size(); index++) {
				if (context[index].variables.find(as.identificator) != context[index].variables.end()) {
					context[index].variables.at(as.identificator) = val.value();
				}
				else if (index == context.size() - 1) {
					context[index].variables.insert({ as.identificator, val.value() });
				}
			}
			return std::nullopt;
		}

		std::optional<Value> Interpreter::VisitReadStatement(ReadStatement& rs) {
			Value value{};
			std::cin >> value.num_value;
			for (std::size_t index = 0; index < context.size(); index++) {
				if (context[index].variables.find(rs.variable_name) != context[index].variables.end()) {
					context[index].variables.at(rs.variable_name) = value;
				}
				else if (index == context.size() - 1) {
					context[index].variables.insert({ rs.variable_name, value });
				}
			}
			return std::nullopt;
		}

		std::optional<Value> Interpreter::VisitExpression(Expression& e) {
			return e.expression->Accept(*this);
		}

		std::optional<Value> Interpreter::VisitLogicsExpression(LogicsExpression& le) {
			auto val1 = le.expression1->Accept(*this);
			auto val2 = le.expression2->Accept(*this);
			switch (le.logics_operation.size()) {
			case 1:
				switch (le.logics_operation[0]) {
				case '<': return Value{ val1.value().num_value < val2.value().num_value ? 1 : 0 };
				case '>': return Value{ val1.value().num_value > val2.value().num_value ? 1 : 0 };
				}
			case 2:
				switch (le.logics_operation[0]) {
				case '=': return Value{ val1.value().num_value == val2.value().num_value ? 1 : 0 };
				case '!':
					switch (le.logics_operation[1]) {
					case '<': return Value{ val1.value().num_value >= val2.value().num_value ? 1 : 0 };
					case '>': return Value{ val1.value().num_value <= val2.value().num_value ? 1 : 0 };
					case '=': return Value{ val1.value().num_value != val2.value().num_value ? 1 : 0 };
					}
				}
			}
		}

		std::optional<Value> Interpreter::VisitAdditive(Additive& a) {
			auto val1 = a.left->Accept(*this);
			auto val2 = a.right->Accept(*this);
			switch (a.operation) {
			case '+': return Value{ val1.value().num_value + val2.value().num_value };
			case '-': return Value{ val1.value().num_value - val2.value().num_value };
			}
		}

		std::optional<Value> Interpreter::VisitMultiplicative(Multiplicative& m) {
			auto val1 = m.left->Accept(*this);
			auto val2 = m.right->Accept(*this);
			switch (m.operation) {
			case '*': return Value{ val1.value().num_value * val2.value().num_value };
			case '/': return Value{ val1.value().num_value / val2.value().num_value };
			}
		}

		std::optional<Value> Interpreter::VisitUnary(Unary& u) {
			auto val = u.right->Accept(*this);
			if (u.operation == '-') {
				val.value().num_value *= -1;
			}
			return Value{ val.value().num_value };
		}

		std::optional<Value> Interpreter::VisitFactor(Factor& f) {
			return f.value->Accept(*this);
		}

		std::optional<Value> Interpreter::VisitBrackets(Brackets& b) {
			return Value{ b.expression->Accept(*this).value().num_value };
		}

		std::optional<Value> Interpreter::VisitLiteral(Literal& l) {
			return Value{ l.value };
		}

		std::optional<Value> Interpreter::VisitIdentificator(Identificator& i) {
			std::string name = i.name;
			for (std::size_t index = 0; index < context.size(); index++) {
				if (context[index].variables.find(i.name) != context[index].variables.end()) {
					return context[index].variables.at(name);
				}
				else if (index == context.size() - 1) {
					em.CollectError(Error(i.line, 11));
				}
			}
			return std::nullopt;
		}
	}
}
